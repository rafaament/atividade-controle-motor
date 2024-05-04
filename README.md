## Utilizando os conceitos abordados em aula, crie um firmware que seja capaz de:
- Via MQTT receber o sentido de giro do motor (horário/anti-horário);
- Via MQTT acionar o motor na intensidade desejada (controle PWM).

# Como funciona?
O raspberry pico através do código em C receberá via MQTT, o sentido do motor (horário/anti-horário) e a intensidade via python

# Linguagem C 
Para compilar o projeto no Windows, execute os seguintes comandos no diretório raiz do projeto
```console
mkdir build
cd build
cmake ..
ninja
```

Para compilar o projeto em Sistemas Operacionais Baseados em Linux, execute os seguintes comandos no diretório raiz do projeto
```console
mkdir build
cd build
cmake ..
make
```

# Linguagem Python
Usar o comando pip para fazer instalação das bibliotecas necessárias

```console
!pip install paho
```


# Curiosidades
Para garantir maior segurança da parte do servidor (raspberry pi) e do lado do client (python) utilizei o conceito de ENV para 
armazenar configurações de endereço de broker, garantindo a segurança das informações