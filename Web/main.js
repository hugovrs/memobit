/* =================================
   MEMORY GAME RANKING
   Arduino + HTML + CSS + JavaScript
================================= */


/* ===== BANCO DE DADOS LOCAL ===== */

function carregarRanking() {

    try {
        const dados = JSON.parse(
            localStorage.getItem("rankingMemoryGame")
        ) || [];

        if (!Array.isArray(dados)) return [];

        return dados.map((jogador, index) => ({
            ...jogador,
            cadastro: Number.isFinite(jogador.cadastro)
                ? jogador.cadastro
                : index
        }));

    } catch (erro) {
        console.error("Erro ao carregar ranking:", erro);
        return [];
    }
}


let jogadores = carregarRanking();

let proximoCadastro = jogadores.reduce(
    (maior, jogador) => Math.max(maior, jogador.cadastro),
    -1
) + 1;


/* ===== ELEMENTOS HTML ===== */

const connectBtn = document.getElementById("connectBtn");
const status = document.getElementById("status");
const statusIndicator = document.getElementById("statusIndicator");

const nomeInput = document.getElementById("playerName");
const rankingList = document.getElementById("rankingList");


/* ===== CONEXÃO SERIAL ===== */

let porta = null;
let leitor = null;
let conectado = false;


/* ===== SALVAR RANKING ===== */

function salvarRanking() {

    localStorage.setItem(
        "rankingMemoryGame",
        JSON.stringify(jogadores)
    );

}


/* ===== ATUALIZAR STATUS ===== */

function atualizarStatus(mensagem, online) {

    status.textContent = mensagem;

    status.style.color = online ? "#4ade80" : "#f87171";

    statusIndicator.style.background =
        online ? "#22c55e" : "#ef4444";

}


/* ===== CONECTAR ARDUINO ===== */

connectBtn.addEventListener("click", conectarArduino);


async function conectarArduino() {

    if (!("serial" in navigator)) {

        atualizarStatus(
            "Navegador não compatível com Web Serial",
            false
        );

        return;
    }

    try {

        porta = await navigator.serial.requestPort();

        await porta.open({
            baudRate: 9600
        });

        conectado = true;

        atualizarStatus("Arduino conectado!", true);

        connectBtn.textContent = "Arduino conectado";
        connectBtn.disabled = true;

        receberDados();

    } catch (erro) {

        console.error("Erro na conexão:", erro);

        atualizarStatus(
            "Conexão cancelada ou indisponível",
            false
        );

    }

}


/* ===== RECEBER DADOS DO ARDUINO ===== */

async function receberDados() {

    const decoder = new TextDecoderStream();

    const leituraFinalizada =
        porta.readable.pipeTo(decoder.writable);

    leitor = decoder.readable.getReader();

    let buffer = "";

    try {

        while (conectado) {

            const { value, done } = await leitor.read();

            if (done) break;

            buffer += value;

            const linhas = buffer.split("\n");

            buffer = linhas.pop();

            for (const linha of linhas) {

                processarResultado(linha.trim());

            }

        }

    } catch (erro) {

        console.error("Erro na leitura serial:", erro);

    } finally {

        if (leitor) {
            leitor.releaseLock();
            leitor = null;
        }

        await leituraFinalizada.catch(() => {});

        conectado = false;

        connectBtn.disabled = false;
        connectBtn.textContent = "Conectar Arduino USB";

        atualizarStatus("Desconectado", false);

        porta = null;

    }

}


/* ===== PROCESSAR RESULTADOS ===== */

function processarResultado(mensagem) {

    console.log("Arduino:", mensagem);

    const resultado = mensagem.match(/^RESULT:(\d+)$/);

    if (!resultado) return;

    const pontos = Number(resultado[1]);

    const nome = nomeInput.value.trim();

    if (!nome) {

        atualizarStatus(
            "Digite o nome do jogador!",
            true
        );

        return;
    }

    // Adiciona jogador ao ranking
    jogadores.push({

        nome: nome,

        pontos: pontos,

        cadastro: proximoCadastro++

    });

    salvarRanking();

    atualizarRanking();

    atualizarStatus(
        `Resultado recebido: ${nome} - ${pontos} pontos`,
        true
    );

    console.log("Resultado registrado!");

}


/* ===== ATUALIZAR PÓDIO ===== */

function atualizarPodio(grupos) {

    const posicoes = [

        {
            jogadores: "firstPlayers",
            pontos: "firstPoints"
        },

        {
            jogadores: "secondPlayers",
            pontos: "secondPoints"
        },

        {
            jogadores: "thirdPlayers",
            pontos: "thirdPoints"
        }

    ];


    // Limpa as posições anteriores
    posicoes.forEach(posicao => {

        document.getElementById(
            posicao.jogadores
        ).innerHTML = "";

        document.getElementById(
            posicao.pontos
        ).textContent = "0 pts";

    });


    // Preenche as três melhores posições
    grupos.slice(0, 3).forEach((grupo, index) => {

        const container = document.getElementById(
            posicoes[index].jogadores
        );

        const pontos = document.getElementById(
            posicoes[index].pontos
        );

        // Limite de três jogadores por posição
        const selecionados = grupo.jogadores.slice(0, 3);

        selecionados.forEach(jogador => {

            const elemento = document.createElement("div");

            elemento.className = "podium-player";

            elemento.textContent = jogador.nome;

            container.appendChild(elemento);

        });

        pontos.textContent = grupo.pontos + " pts";

    });

}


/* ===== ATUALIZAR RANKING GERAL ===== */

function atualizarRanking() {

    // Ordena por pontuação e ordem de cadastro
    jogadores.sort((a, b) => {

        return b.pontos - a.pontos ||
               a.cadastro - b.cadastro;

    });


    // Agrupa jogadores com a mesma pontuação
    const grupos = [];

    jogadores.forEach(jogador => {

        let grupo = grupos.find(
            g => g.pontos === jogador.pontos
        );

        if (!grupo) {

            grupo = {

                pontos: jogador.pontos,

                jogadores: []

            };

            grupos.push(grupo);

        }

        grupo.jogadores.push(jogador);

    });


    // Atualiza o pódio
    atualizarPodio(grupos);


    // Atualiza a tabela geral
    rankingList.innerHTML = "";


    grupos.forEach((grupo, index) => {

        grupo.jogadores.forEach(jogador => {

            const linha = document.createElement("tr");

            const posicao = document.createElement("td");
            const nome = document.createElement("td");
            const pontos = document.createElement("td");

            // Jogadores empatados compartilham posição
            posicao.textContent = `${index + 1}º`;

            nome.textContent = jogador.nome;

            pontos.textContent = `${jogador.pontos} pts`;

            linha.append(
                posicao,
                nome,
                pontos
            );

            rankingList.appendChild(linha);

        });

    });

}


/* ===== INICIALIZAÇÃO ===== */

// Recupera e mostra os dados salvos
atualizarRanking();
