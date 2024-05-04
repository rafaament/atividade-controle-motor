#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pwm.h"
#include "pico/time.h"
#include "pico/unique_id.h"
#include "pico/binary_info.h"
#include "pico/stdio/driver.h"
#include "hardware/gpio.h"
#include "pico/mqtt.h"

#define PWM_PIN 18
#define PWM_RANGE 255 
#define BUF_SIZE 128
#define MQTT_TOPIC_DIRECTION "motor_direction"
#define MQTT_TOPIC_INTENSITY "motor_intensity"

#define A1 16
#define A2 17

void control_motor(uint32_t direction, uint32_t intensity) {
    gpio_init(PWM_PIN);
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_wrap(slice_num, PWM_RANGE);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, intensity);
    pwm_set_enabled(slice_num, true);
   
    gpio_init(A1);
    gpio_init(A2);
 
    gpio_set_dir(A1, GPIO_OUT);
    gpio_set_dir(A2, GPIO_OUT);
 
    gpio_put(A1, direction);
    gpio_put(A2, !direction);
}

int main() {
    stdio_init_all();

    set_sys_clock_khz(125000, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    uint64_t unique_id = get_unique_board_id();
    printf("Raspberry Pi Pico ID: %016llx\n", unique_id);
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    mqtt_config config = {
        .host = getenv("MQTT_BROKER"),
        .port = 1883,
        .ssid = getenv("WIFI_NAME"),
        .password = getenv("WIFI_PASSWORD")
    };

    mqtt_client_t *client = mqtt_init(&config);

    mqtt_connect(client);

    while (true) {
        mqtt_msg_t *msg_direction = mqtt_msg_received(client, MQTT_TOPIC_DIRECTION);
        if (msg_direction != NULL) {
            printf("Direction MQTT message received: %s\n", msg_direction->data);
            uint32_t direction = msg_direction->data[0] - '0';
            free(msg_direction);

            mqtt_msg_t *msg_intensity = mqtt_msg_received(client, MQTT_TOPIC_INTENSITY);
            uint32_t intensity = 0;
            if (msg_intensity != NULL) {
                printf("Intensity MQTT message received: %s\n", msg_intensity->data);
                intensity = atoi(msg_intensity->data);
                free(msg_intensity);
            }

            control_motor(direction, intensity);

            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            sleep_ms(500);
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
        }
        sleep_ms(100);
    }

    return 0;
}
