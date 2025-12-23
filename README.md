# Projeto de Programação II - Jogo 2D Modular

## Descrição
Este repositório contém um jogo desenvolvido em linguagem **C** com a biblioteca **Allegro 5**. O projeto foi estruturado de forma modular, separando a lógica de jogo, renderização de mapas e comportamento de entidades em diferentes arquivos de implementação.

## Estrutura do Diretório
O projeto utiliza os seguintes módulos técnicos:

* **Módulos de Código:** `main.c`, `player.c`, `enemy.c`, `map.c` e seus respectivos headers (`.h`).
* **Assets de Imagem:** `amogus.png`, `amogus inimigo.png` e `tiles.png`.
* **Configurações de Mapa:** Arquivos de texto `map.txt` a `map5.txt`.
* **Recursos de Fonte:** `GROBOLD.ttf`.
* **Ambiente de Desenvolvimento:** Arquivos de projeto Code::Blocks para Windows (`allegro32.cbp`, `allegro64.cbp`) e Linux (`allegro_linux.cbp`).

## Especificações de Build
* **Linguagem:** C (Padrão C99/C11).
* **Dependências (Allegro 5):**
    * `allegro_image`
    * `allegro_font`
    * `allegro_ttf`
    * `allegro_audio`
    * `allegro_acodec`
    * `allegro_primitives`

## Compilação e Execução

### Via Code::Blocks
Abra o arquivo `.cbp` compatível com seu sistema e compile através da interface da IDE. Certifique-se de que os diretórios de busca (*Search Directories*) apontam para o local da Allegro no seu sistema.

### Via Terminal (GCC)
Para realizar o build manual unindo todos os módulos:

gcc main.c player.c enemy.c map.c -o jogo_prog2 -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_primitives

## Autores
* [Kaio Pimentel dos Santos]
* [Kwame Nkrumah Mawusi Gil Leal]
* [Rafael Pinheiro Rosa]
