# 🧠 MemoBit

Desafie sua memória, acompanhe a sequência e conquiste seu lugar no Hall da Fama!

O MemoBit é um projeto de jogo eletrônico de memória inspirado no clássico Genius (Simon). O objetivo é testar a capacidade de memorização dos jogadores por meio de sequências de símbolos que precisam ser reproduzidas corretamente.

O projeto combina programação, eletrônica e desenvolvimento web, integrando um Arduino a uma interface digital para proporcionar uma experiência interativa.

🎮 Sobre o jogo

O MemoBit apresenta sequências de três símbolos:

- ◻️ Quadrado
- ❌ X
- ⭕ Círculo

O jogador deve observar a sequência apresentada pelo display e reproduzi-la utilizando os botões físicos conectados ao Arduino.

A cada fase, a sequência se torna maior, aumentando gradualmente o nível de dificuldade.

⚙️ Funcionalidades

- [x] Exibição de símbolos em display LED MAX7219.
- [x] Três botões independentes para interação.
- [x] Sistema de início de partida.
- [x] Sequências progressivas de memória.
- [x] Identificação de acertos e erros.
- [x] Feedback visual para o jogador.
- [x] Integração entre Arduino e sistema web.
- [x] Sistema de pontuação.
- [x] Cadastro de jogadores.
- [x] Ranking de pontuações com pódio.
- [x] Organização de jogadores empatados por ordem de cadastro.

🛠️ Tecnologias utilizadas

Hardware

- Arduino Uno
- Display LED MAX7219 (matriz 8x8)
- 3 botões físicos
- Jumpers e protoboard

Software

- C/C++ (Arduino)
- JavaScript
- HTML
- CSS
- Comunicação serial entre Arduino e computador

🔌 Funcionamento

O funcionamento do MemoBit é dividido em três etapas principais:

1. Geração da sequência

O Arduino apresenta uma sequência de símbolos no display MAX7219.

2. Interação do jogador

O jogador utiliza os três botões físicos para reproduzir a sequência apresentada.

3. Pontuação e classificação

O sistema registra o desempenho do jogador e atualiza o ranking, permitindo acompanhar os melhores resultados no Hall da Fama.

🏆 Hall da Fama

O MemoBit possui um sistema de classificação que organiza os jogadores de acordo com suas pontuações.

O sistema também apresenta um pódio com as três primeiras posições, permitindo até três jogadores por posição em caso de empate, respeitando a ordem de cadastro.

📁 Estrutura do projeto

<pre>
MemoBit/
├── Arduino/
│   └── memobit.ino
├── Web/
│   ├── index.html
│   ├── main.js
│   └── style.css
└── README.md
</pre>

🚀 Objetivo do projeto

O MemoBit foi desenvolvido com o propósito de explorar conceitos de programação, eletrônica, lógica computacional e integração entre hardware e software.

Além de proporcionar entretenimento, o projeto representa uma oportunidade de aplicar conhecimentos de desenvolvimento de sistemas em uma aplicação prática.

👨‍💻 Desenvolvimento

Projeto: MemoBit
<br>
Desenvolvedor: Hugo Veras

Desenvolvido como projeto de aprendizagem, unindo tecnologia, criatividade e desafios de lógica.

---

<p align="center">
  🧠 <strong>MemoBit — Sua memória é o seu maior recurso!</strong>
</p>
