<div align='center'>

# Breakout na DE1 SoC - Desenvolvendo um Jogo 
</div>

## Índice:

  - [1. Sobre](#1-sobre)
    - [1.1. Objetivo](#11-objetivo)
    - [1.2. Requisitos do Problema](#12-requisitos-do-problema)
    - [1.3. Plataformas e Softwares utilizados](#13-plataformas-e-softwares-utilizados)
    - [1.4. Hardware da DE1-SoC](#14-hardware-da-de1-soc)
  - [2. Visão Geral](#2-visão-geral)
    - [2.1. Funcionamento do Sistema](#21-funcionamento-do-sistema)
    - [2.2. Funcionamento do Jogo](#22-funcionamento-do-jogo)
    - [2.3. Biblioteca lintelfpgaup](#23-biblioteca-lintelfpgaup)
  - [3. Interface do usuário](#3-interface-do-usuário)
  - [4. Exibição de imagens na tela](#4-exibição-de-imagens-na-tela)
    - [4.1. Pixel buffer](#41-pixel-buffer)
    - [4.2. Double Buffering](#42-double-buffering)
  - [5. Acelerômetro, Saída VGA e Botões](#5-acelerômetro-saída-vga-e-botões)
    - [5.1. Acelerômetro](#51-acelerômetro)
    - [5.2. Saída VGA](#52-saída-vga)
    - [5.3. Botões](#53-botões)
  - [6. Funcionamento dos elementos do jogo](#6-funcionamento-dos-elementos-do-jogo)
    - [6.1. Movimentação da bola](#61-movimentação-da-bola)
    - [6.2. Colisão](#62-colisão)
    - [6.3. Movimentação da raquete](#63-movimentação-da-raquete)
    - [6.4. Blocos](#64-blocos)
    - [6.5. Pausa](#65-pausa)
    - [6.6. Vidas](#66-vidas)
  - [7. Testes](#7-testes)
  - [8. Conclusão](#8-conclusão)
  - [9. Makefile](#9-makefile)
  - [10. Como executar](#10-como-executar)
    - [DE1-SoC](#de1-soc)
    - [Compilação do projeto](#compilação-do-projeto)
  - [11. Referências](#11-referências)

## Equipe

Gabriel Baptista: [@BaptistaGabriel](https://github.com/BaptistaGabriel)

Jeferson Almeida: [@TristanJeferson](https://github.com/TristanJeferson)

Kauan Farias: [@kakafariaZ](https://github.com/kakafariaZ)

Márcio Roberto: [@MarcioDzn](https://github.com/MarcioDzn)

## 1. Sobre 

### 1.1. Objetivo 

O projeto tem como objetivo o desenvolvimento de um jogo inspirado no clássico Breakout, utilizando a linguagem de programação C e sendo executado na plataforma DE1-SoC. O controle do jogo é realizado através do movimento do jogador, que é capturado pelo acelerômetro da placa, além da interação por meio dos botões disponíveis. A interface gráfica é exibida em um monitor CRT utilizando a interface VGA.

### 1.2. Requisitos do Problema 

- Desenvolvimento do jogo em linguagem C.
- Utilização da plataforma DE1-SoC.
- Controle do jogo através do movimento capturado pelo acelerômetro e dos botões disponíveis.
- Exibição da interface gráfica do jogo em um monitor CRT via interface VGA.


### 1.3. Plataformas e Softwares utilizados

- Placa DE1-SoC
- Monitor CRT Dell E773C
- Visual Studio Code


### 1.4. Hardware da DE1-SoC 

#### FPGA

- Dispositivo Altera Cyclone® V SE 5CSEMA5F31C6N.
- Dispositivo de configuração serial Altera - EPCS128.
- USB-Blaster II embarcado para programação; Modo JTAG.
- 64MB de SDRAM (bus de dados de 16 bits).
- 4 botões de pressão.
- 10 chaves deslizantes.
- 10 LEDs de usuário vermelhos.
- Seis displays de 7 segmentos.
- Quatro fontes de clock de 50MHz do gerador de clock.
- Codec de áudio de qualidade de CD de 24 bits com entradas de linha, saídas de linha e entradas de microfone.
- DAC VGA (conversores analógico-digitais de 8 bits de alta velocidade triplos) com conector VGA de saída.
- Decodificador de TV (NTSC/PAL/SECAM) e conector de entrada de TV.
- Conector PS/2 para mouse/teclado.
- Receptor de IR e emissor de IR.
- Dois cabeçalhos de expansão de 40 pinos com proteção de diodo.
Conversor A/D, interface SPI de 4 pinos com FPGA.

#### HPS (Hard Processor System)

- Processador ARM Cortex-A9 MPCore dual-core de 800MHz.
- 1GB de DDR3 SDRAM (bus de dados de 32 bits).
- PHY Gigabit Ethernet com conector RJ45.
- 2 portas USB Host, conector USB tipo A normal.
- Soquete para cartão micro SD.
- Acelerômetro (interface I2C + interrupção).
- UART para USB, conector USB Mini-B.
- Botão de reset quente e botão de reset frio.
- Um botão do usuário e um LED do usuário.
- Cabeçalho de expansão LTC 2x7.

Neste projeto, os componentes utilizados na foram os botões de pressão, o VGA e o acelerômetro. Os botões permitem a interação do jogador, o VGA permite a exibição do game no monitor CRT e o acelerômetro captura os movimentos do jogador.


## 2. Visão Geral 

### 2.1. Funcionamento do Sistema

Inicialmente, um dispositivo externo conectado via SSH é inicializado com o objetivo de solicitar a execução do jogo na placa. Após o estabelecimento da conexão com a placa, o jogo é inicializado, seguido pela abertura dos dispositivos necessários e configuração de suas propriedades específicas.
Em seguida, ocorre a renderização dos elementos estáticos e dinâmicos,como a raquete, as paredes, os blocos, as vidas e a bola. Ao concluir a execução do jogo, os dispositivos são fechados, encerrando o processo de forma adequada e organizada. O fluxograma abaixo (Figura 1) oferece uma ilustração das etapas descritas anteriormente.

<div align='center'>
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/3ed5fcfa-6488-4a33-9d6a-c8da399282bf)

Figura 1. Fluxograma de funcionamento do sistema.
</div>

<div align='center'>

</div>

### 2.2. Funcionamento do Jogo 

O fluxograma abaixo (Figura 2) exemplifica o funcionamento geral do jogo.

<div align='center'>
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/91a161b0-8116-488b-84be-4b2ab6b8222e)

Figura 2. Fluxograma do funcionamento do jogo.
</div>

### 2.3. Biblioteca lintelfpgaup 

A distribuição Linux do DE1-SoC-UP fornece módulos do kernel já existentes a fim de permitir uma comunicação com seus módulos de hardware. A partir de uma interface baseada em arquivo, fornecida por cada módulo, é possível ler e escrever informações no driver do dispositivo, ou ler dados dele. 
Os módulos de kernel utilizados no projeto foram: KEY, para acessar a porta referente aos botões; video, para manipular a porta de saída de vídeo VGA; e accel, para ter acesso à porta do acelerômetro 3-D.
Por meio de uma biblioteca intelfpgaup, disponibilizada para a linguagem de programação C, é possível manipular os módulos supracitados, a partir de funções específicas de configuração, leitura e escrita.

## 3. Interface do usuário 

O projeto incorpora diversas telas, abrangendo a tela de início, de jogo em andamento, de vitória e de derrota. Estas interfaces são gerenciadas por dois botões: "key 1" e "pause key". Cada tela é acionada por eventos específicos, promovendo uma experiência interativa ao usuário. O diagrama associado (Figura 3) oferece uma representação clara e concisa da estrutura e das interações do projeto. 
Ademais, é possível identificar a localização dos botões "key 1" e "pause key" mencionados no diagrama (Imagem 1).

<div align='center'>
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/a2c42205-3ca7-4a81-b942-5c42e9f1822a)

Figura 3. Diagrama de telas do game.
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/ec9f5360-5de0-459d-88bb-d89487fff1e9)

Imagem 1. Botões utilizados pelo usuário.
</div>

## 4. Exibição de imagens na tela 
### 4.1. Pixel buffer 

O pixel buffer é responsável por armazenar os dados referentes à cor de um pixel em determinada posição da tela. A resolução fornecida por esse buffer é de 320x240, entretanto, como o monitor utilizado como interface de saída tem resolução de 640x480, cada valor do pixel é duplicado nas direções x e y, ou seja, um pixel passa a ser exibido como 2x2 pixels. Nesse sentido, ao tentar exibir um valor na posição (0, 0) do pixel buffer na tela, o mesmo valor será exibido também nas coordenadas (0, 1), (1, 0) e (1, 1). Este comportamento é ilustrado a seguir (Figura 4) 
<div align='center'>

![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/7e9a3265-b21e-48a9-adb9-2d004b26c08a)

Figura 4. Exibição de dados no monior.
</div>

### 4.2. Double Buffering 

Para melhorar a fluidez e a qualidade de exibição dos dados, utiliza-se a técnica conhecida como Double-Buffering, na qual dois pixel buffers são utilizados, sendo um para guardar os dados que devem ser exibidos e outro para permitir sua exibição. Em outras palavras, enquanto uma informação está sendo exibida na tela outra está sendo preparada em segundo plano.
Dois registradores específicos são utilizados para referenciar os pixel buffers, o Buffer register e o Backbuffer register, os quais guardam os endereços dos buffers em questão. Sempre que deseja-se exibir um dado na tela, o mesmo deve ser armazenado no buffer referente ao Backbuffer register. Em seguida, deve-se realizar um swap, a fim de trocar os conteúdos dos registradores supracitados. A informação do Backbuffer passa para o Buffer register, que armazena o buffer responsável pela exibição de fato dos dados.
Dessa forma, enquanto uma imagem armazenada no pixel buffer do Buffer register está sendo exibida, uma nova imagem pode estar sendo desenhada no pixel buffer referente ao Backbuffer register. A troca, ou swap, ocorre apenas no fim de um ciclo de desenho na tela, ou seja, quando o último pixel do pixel buffer ligado ao Buffer register é exibido. Esse tempo é chamado de sincronização vertical, e ocorre a cada 1/60 segundos.

## 5. Acelerômetro, Saída VGA e Botões 

O acelerômetro, o VGA e os botões são dispositivos que podem ser manipulados pela biblioteca lintelfpgaup. Todos esses módulos necessitam ser abertos antes de serem manuseados, e fechados antes do fim do processo. As suas respectivas funções de abertura são: accel_open(), video_open() e key_open(). Já as funções de fechamento são: accel_close(), video_close() e key_close().

### 5.1. Acelerômetro 

O módulo referente ao acelerômetro fornece funções para configuração e leitura de dados do dispositivo 3D. A função accel_format() permite ajustar o formato dos dados de aceleração, a partir da resolução total e do G range. Além disso, é necessário calibrar o aparelho, a fim de manter a posição horizontal como base para leitura de informações, com a accel_calibrate().
A leitura de dados do dispositivo se dá a partir da função accel_read(), que recebe uma série de ponteiros como parâmetros, os quais recebem o retorno de dados específicos, dentre os quais estão as medições nas direções x, y e z. No projeto em questão, fez-se necessário utilizar apenas os dados de movimentação horizontal do acelerômetro, ou seja, os valores do eixo x.

### 5.2. Saída VGA 

Para transmitir dados a serem exibidos no monitor, utilizou-se o padrão VGA. A partir disso, a fim de manipular o dispositivo VGA da maneira correta, fez-se uso das funções fornecidas pela biblioteca lintelfpgaup.
A exibição de informações é feita a partir da escrita do pixel buffer, o que pode ser feito utilizando a função video_box(), que recebe como parâmetros a posição inicial e final dos pixels, bem como a cor do que será exibido. Além disso, para apagar as informações disponíveis neste buffer, utiliza-se a função video_clear().
Entretanto, ao escrever ou apagar informações, os valores alterados são do pixel buffer referenciado pelo Backbuffer, sendo necessário realizar um swap para que passe a ser referenciado pelo Buffer register. Dessa forma, a função video_show() realiza essa troca de buffers, permitindo que o valor desejado seja de fato exibido.

### 5.3. Botões

A fim de ler corretamente os botões, utiliza-se a função KEY_read(), que recebe como parâmetro um ponteiro, que obtém como retorno 4 bits, cada um representando um botão em específico. Quando um botão é pressionado seu respectivo bit é 1, do contrário é 0, o que permite que verificações sejam feitas periodicamente para saber se um botão foi pressionado.
Nesse sentido, cada botão pressionado singularmente está associado a um valor inteiro em específico, sendo: 1 para o botão 0; 2 para o botão 1; 4 para o botão 2 e 8 para o botão 3. Dessa forma, a partir de comparações é possível saber se um botão em específico foi pressionado.

## 6. Funcionamento dos elementos do jogo 

### 6.1. Movimentação da bola

Para realizar a movimentação da bola no cenário modificou-se suas posições horizontal e vertical ao longo do tempo. Isso se deu a partir do incremento ou decremento dessas posições a uma taxa constante, tomando com base a direção em que a bola deveria seguir.

### 6.2. Colisão 

Um requisito fundamental para o correto funcionamento do jogo é garantir a colisão da bola com os demais elementos do cenário, como as paredes, os blocos e a “raquete”. A implementação dessa funcionalidade se deu a partir de uma antecipação do movimento da bolinha, ou seja, da verificação da posição futura em que a bolinha vai estar. 
Para tanto, analisou-se pixels adjacentes à posição atual da bolinha, verificando se algum deles se sobrepôs a algum pixel pertencente a um elemento do cenário, seja horizontalmente ou verticalmente. 
Assim, se o pixel referente à bola estiver na posição (x,y), as demais posições (x+1, y), (x-1, y), (x, y+1) e (x, y-1) são verificadas, caso haja interceptação de algum objeto, uma colisão é identificada. Ademais, seguindo o mesmo princípio, as posições diagonais à bola também são averiguadas, a fim de detectar colisões nas bordas dos objetos,sendo elas (x+1, y+1), (x-1, y+1), (x+1, y-1) e (x-1, y-1) (Figura 5).

<div align='center'>
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/c6f7f7e6-55ec-46b4-aeb5-2e7fae90a975)

Figura 5. Área de verificação da bola.
</div>

A partir do momento que uma colisão é identificada, a direção de movimentação da bola é invertida. Dessa forma, para colisões verticais, a velocidade horizontal permanece a mesma, mas a velocidade vertical tem seu sinal invertido. Para colisões horizontais, a velocidade vertical não se altera, mas a horizontal tem seu sinal trocado. Já para colisões diagonais tanto a velocidade horizontal quanto a vertical tem seus sinais invertidos (Figura 6).

<div align='center'>
  
![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/469b0651-2cec-450e-9369-f1245a6c3f72)

Figura 6. Direção da bola após a colisão com blocos.
</div>

Ademais, para as colisões entre a bola e a raquete, há um comportamento exclusivo implementado. O comprimento da "raquete" é dividido em três zonas distintas. Uma colisão na primeira zona (parte mais à esquerda da raquete) faz com que a bola se desloque para a esquerda e para cima. Já uma colisão na segunda zona (parte mais central da raquete) resulta em um movimento vertical ascendente da bola. Por fim, uma colisão na terceira zona (parte mais à direita da raquete) faz com que a bola se mova para a esquerda e para cima (Figura 7). 

<div align='center'>

![image](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/b3938712-5385-42a9-b6d1-1cd5a6001014)

Figura 7. Direção da bola após a colisão com a raquete.
</div>

### 6.3. Movimentação da raquete 

Para movimentar horizontalmente a raquete utilizou-se os valores lidos do acelerômetro referentes ao eixo x. Dessa forma, valores positivos advindos do dispositivo 3D eram incrementados à posição inicial da raquete, movimentando-a para a direita. De modo análogo, valores negativos eram decrementados à posição base, resultando em um movimento para a esquerda.
Ademais, para evitar que a raquete saísse do campo de visão do jogador, limitou-se sua posição inicial e final, mantendo-a sempre entre os limites horizontais da tela.

### 6.4. Blocos 

A fim de exibir os blocos quebráveis no cenário criou-se, utilizando a função fillBricks(), um array de structs, onde cada elemento contém informações sobre um bloco diferente, como posição inicial e final, cor e se tem status de visível ou não. Os blocos indicados como visíveis são então exibidos na tela em n linhas e m colunas, a partir da função renderBricks().
Sempre que a bola “toca” em um desses blocos o seu status de visível é alterado para não visível, removendo-o da tela imediatamente e impedindo que a bola o “toque” novamente.

### 6.5. Pausa 

Um sistema de pausa também foi implementado, sendo um dos requisitos do projeto. Para tanto, realizou-se a leitura constante dos estados dos botões dentro do loop principal, a fim de identificar um possível aperto.
Sempre que o respectivo botão de pausa é apertado, o sistema fica “preso” em um loop específico, o qual impede novas renderizações na tela, mantendo o último estado fixo, até o jogador despausar o sistema. Nesse loop também há a verificação do estado do botão, onde também averigua possíveis apertos. Caso o usuário aperte novamente o mesmo botão, o sistema “despausa”, saindo do loop e executando novamente o laço principal.

### 6.6. Vidas 

Uma funcionalidade implementada para tornar o jogo mais interessante é o sistema de vidas. O jogador inicialmente tem 3 vidas, representadas na parte superior da tela como 3 traços vermelhos. As vidas podem ser perdidas conforme a bola colide com a parte inferior do cenário, e quando isso acontece, um dos indicadores some. Após a perda das 3 vidas, o jogo é finalizado como uma derrota.

## 7. Testes 

Para validar a eficácia e robustez do produto desenvolvido no projeto, foram realizados diversos testes. Estes testes visam verificar o funcionamento adequado de todas as funcionalidades implementadas, bem como garantir que o jogo atenda aos requisitos e expectativas estabelecidas. Os resultados esperados desses testes foram organizados em duas tabelas distintas: a Tabela 2 contém os resultados esperados para os testes de colisão, enquanto a Tabela 1 apresenta os resultados esperados para os testes gerais.

<div align='center'>

### Testes gerais

| **Teste** | **Acontecimento esperado** |
|:---:|:---:|
| Pausar o Jogo | Jogo Pausar |
| Colidir com o chão | Perder Vidas |
| Colidir 3 vezes com o chão | Perder o Jogo |
| Apertar  Key 1 após perder/ganhar o jogo | Voltar para a tela de inicio |
| Eliminar todos os blocos | Ganhar o jogo |

Tabela 1. Testes gerais.


### Testes relacionados a colisão

| **Colisão** | **Acontecimento esperado** |
|:---:|:---:|
| Colisão entre a bola e o lado esquerdo da raquete | Bola vai para o lado esquerdo e sobe |
| Colisão entre a bola e o meio da raquete | Bola sobe somente com o eixo Y |
| Colisão entre a bola e o lado direito da raquete | Bola vai para o lado direito e sobe |
| Colisão entre a bola e o canto inferior esquerdo do bloco | Quebra o bloco |
| Colisão entre a bola e o canto superior direito do bloco | Quebra o bloco |
| Colisão entre a bola e o canto superior esquerdo do bloco | Quebra o bloco |
| Colisão entre a bola e o canto inferior direito do bloco | Quebra o bloco |
| Colisão entre a bola e a base do bloco | Quebra o bloco |
|  Colisão entre a bola e o lado esquerdo do bloco | Quebra o bloco |
| Colisão entre a bola e o lado direito bloco | Quebra o bloco |
| Colisão entre a bola e o topo bloco  | Quebra o bloco |

Tabela 2. Testes relacionados a colisão.
</div>

Em seguida, serão apresentados gifs que demonstram a execução dos testes realizados.

| Colisão entre a bola e a base do bloco ![Colisão bottom bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/10d1ccb5-6ded-4ffc-adaa-89f5d6312df2) | Colisão entre a bola e o canto inferior direito do bloco ![Colisão canto inferior direito do bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/29fc3ffe-3aec-4d26-8e8e-3deb576288bc) | Colisão entre a bola e o canto inferior esquerdo do bloco ![Colisão canto inferior esquerdo do bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/f2d3d6ce-d7b1-433d-8751-6e98b8173cfd) |
|:---:|:---:|:---:|
| Colisão entre a bola e o canto superior direito do bloco ![Colisão canto superior direito do bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/828b67f0-8e2c-4478-8a3f-998c1f4254cb) | Colisão entre a bola e o canto superior esquerdo do bloco ![Colisão canto superior esquerdo do bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/4edecd35-032a-48a6-a59d-1cbcc6b1f385) | Colisão entre a bola e o lado direito barra ![Colisão lado direito barra](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/75198298-0115-4343-80bd-23ad47eb816a) |
| Colisão entre a bola e o lado esquerdo do bloco ![Colisão lado esquedo bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/252ded13-cce8-4b56-982e-b3978a1bdb41) | Colisão entre a bola e o lado direito bloco ![Colisão lado direito do bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/9bad7726-91c8-4f91-b896-6f2e0af46548) | Colisão entre a bola e o lado esquedo da barra ![Colisão lado esquedo barra (1)](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/a0634c0e-db06-4fa8-9f18-19e388f5647e) |
| Colisão entre a bola e o  meio da barra ![Colisão meio barra](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/d95235e7-1b3b-424a-9715-53e6646719b5) | Colisão entre a bola e o topo bloco ![Colisão topo bloco](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/edb334d5-a8a6-470c-a32b-f0aea2563db8) | Ganhando o Jogo ![Ganhando o Jogo (1)](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/7979e7b6-6fb9-4088-b959-f8ecff7b6ac0) |
| Testando as 3 vidas ![teste de vidas (1)-min](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/b6f0627f-a35d-4d35-af1e-47e2424c0c78) | Testando a pausa ![Teste Pause](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/87533bb3-e5d8-4ec7-bf16-11757a4979f2) | Voltando a tela inicial apos perder/ganhar ![voltando a tela inicial apos perder](https://github.com/BaptistaGabriel/Breakout-na-DE1-SoC--Desenvolvendo-um-Jogo-em-uma-Plataforma-ARM/assets/91295529/a0067ea0-f689-462c-841a-9103db077129) |


## 8. Conclusão 



Ao término do desenvolvimento, todos os requisitos do projeto foram cumpridos e o sistema operou conforme o esperado. Este resultado é atribuído à aplicação adequada dos conceitos adquiridos durante o período de desenvolvimento.

Nesse contexto, a integração dos elementos essenciais resultou em um sistema funcional que apresenta corretamente as funcionalidades necessárias. Esses elementos incluem o uso do acelerômetro para captura de movimento do jogador, as colisões dentro do jogo e utilização dos botões disponíveis na placa DE1-SoC para interação do usuário.

## 9. Makefile 

Para simplificar o processo de compilação e execução do programa em C, foi criado um Makefile, o qual executa as seguintes operações:

- **Compilação**: Converte o código fonte C (.c) em um arquivo objeto (.o) usando o compilador C.
- **Linkagem**: Liga o arquivo objeto gerado na etapa anterior e gera o executável final.
- **Execução**: Inicia a execução do programa.

## 10. Como executar 
### DE1-SoC
Conecte-se com a placa

### Compilação do projeto

1) Navegue até o diretório do projeto usando o terminal.
2) Execute o comando a seguir:
```$ make all``` 

## 11. Referências 

DE1-SoC Computer System with ARM* Cortex* A9: https://ftp.intel.com/Public/Pub/fpgaup/pub/Intel_Material/18.1/Computer_Systems/DE1-SoC/DE1-SoC_Computer_ARM.pdf - Acesso em 24 fev. 2024.

Using Linux* on DE-series Boards: https://ftp.intel.com/Public/Pub/fpgaup/pub/Intel_Material/17.0/Tutorials/Linux_On_DE_Series_Boards.pdf - Acesso em 24 fev. 2024.

