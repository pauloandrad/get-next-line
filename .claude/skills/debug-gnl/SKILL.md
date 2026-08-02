---
name: debug-gnl
description: Depuração de falhas do testador GNL um teste por vez com saída estruturada e revisão de diff
---

# Skill de Depuração GNL

Esta skill depura falhas do testador GNL (Get Next Line) seguindo um framework rigoroso de um-teste-por-vez. Nunca processe múltiplas falhas de teste em uma iteração.

## Configuração
- **Localização do testador:** diretório `gnlTester/`
- **Comando de execução:** `wsl -e bash -c "cd /home/pauloh8755/projects/common-core/get-next-line/gnlTester && make 2>&1"`
- **Arquivos de código (`src`):** `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`
- **Arquivos de entrada de teste (`files`):** diretório `gnlTester/files/`
- **Roteiros de teste (`tests`):** `gnlTester/tests/mandatory.cpp`, `gnlTester/tests/bonus.cpp`
- **Sandbox de reprodução para GDB:** diretório `.debug/` (preparado por esta skill, consumido pelo agente `debug-gdb`)

## Arquitetura do Worktree

Três áreas com papéis distintos:
- **`src`** (raiz do repositório): `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`. Única fonte de verdade do código — toda correção definitiva acontece aqui, nunca em `.debug/`.
- **`gnlTester/`** (framework de teste, não se edita): `files/` guarda as entradas fixas por nome de teste; `tests/mandatory.cpp`/`bonus.cpp` são os roteiros que abrem cada arquivo e chamam `gnl(fd, esperado)` em sequência — é ali que se descobre o BUFFER_SIZE, a entrada e a sequência exata que reproduz a falha; `Makefile` mostra como cada BUFFER_SIZE é compilado (`-D BUFFER_SIZE=1|42|10000000`, `-g3 -gdwarf-4`).
- **`.debug/`** (sandbox de reprodução, descartável por cenário): montado por esta skill a cada teste que precisa de investigação via GDB, e limpo assim que o teste correspondente passa.

## Framework Um-Teste-Por-Vez

### Passo 1: Executar Testador e Capturar Saída
Execute o comando do testador e capture a saída completa. Identifique o PRIMEIRO teste que falha (aparece após um `OK` anterior ou no início). Pare após identificar uma falha.

### Passo 2: Extrair Metadados do Teste
Para o teste que falha, extraia e apresente ao usuário:
- **Nome do teste:** (ex: `files/nl`, `files/41_no_nl`)
- **BUFFER_SIZE:** (ex: 1, 42, 10M)
- **Caminho do arquivo de entrada:** (ex: `gnlTester/files/nl`)
- **Comportamento esperado:** Baseado no nome do teste (ex: "ler arquivo com nova linha")
- **Erro/saída real:** Erro de Valgrind, leitura inválida/escrita, segfault, etc.
- **Localização do erro:** Arquivo, função, número de linha do stack trace
- **Frase do cenário:** Uma frase descrevendo o que deu errado (ex: "Buffer overflow ao ler linha que contém nova linha")

### Passo 3: Apresentar ao Usuário (Antes de Tocar no Código)
Analise a implementação do primeiro teste que falhou em gnlTeste/tests e forneça metadados extraídos ao usuário em formato claro:
```
## Análise da Falha do Teste

**Teste:** [nome]
**BUFFER_SIZE:** [tamanho]
**Entrada:** [caminho ou descrição]
**Cenário:** [frase de uma sentença]

**Erro:** [Erro do Valgrind ou tipo de crash]
**Localização:** `[arquivo]:[linha]` em função `[func]()`
**Detalhes:** [stack trace relevante ou informações de memória]
**Faltam:**  [Quantidade de testes que faltam] testes para finalizar a entrega
```

Aguarde o usuário confirmar antes de prosseguir para a preparação do sandbox de depuração.

### Passo 4: Preparar Sandbox de Depuração em `.debug/`
Monte em `.debug/` a reprodução mínima que o agente `debug-gdb` vai investigar. Regra central: replique fielmente, nunca reimplemente de memória.

**Replicar o código de `src`:**
1. Leia o conteúdo ATUAL de `get_next_line.c`, `get_next_line_utils.c` e `get_next_line.h` (nunca reaproveite uma cópia de sessão anterior).
2. Prefira `#include` direto dos arquivos reais a partir de `.debug/` (ex: `#include "../get_next_line.c"`) em vez de colar texto manualmente, garantindo fidelidade 100%. Só copie o texto literal, sem alterar uma linha, se o `#include` do `.c` causar conflito de símbolos.
3. Se o bug envolve estado entre chamadas (`static`/stash), inclua o arquivo inteiro para preservar esse estado entre chamadas sucessivas no repro.
4. Defina `BUFFER_SIZE` via `-D BUFFER_SIZE=N` na compilação, igual ao teste que está sendo reproduzido.

**Replicar o caso de teste do tester:**
1. Localize em `gnlTester/tests/mandatory.cpp` (ou `bonus.cpp`) o bloco `title("files/NOME: ")` do teste que falhou.
2. Extraia o arquivo de entrada, a sequência de chamadas `gnl(fd, esperado)` e o BUFFER_SIZE (via `Makefile`: `gnlTest_bs1`/`bs42`/`bs10M` → 1/42/10000000).
3. Copie o arquivo de entrada real de `gnlTester/files/NOME` para `.debug/` com `cp` — nunca digite o conteúdo à mão (pares `*_no_nl`/`*_with_nl` são sensíveis a byte exato).
4. Escreva um `main()` mínimo em `.debug/<nome_do_teste>.c` que abre o arquivo copiado e chama a função sob teste o mesmo número de vezes, na mesma ordem, imprimindo cada retorno.

Ao final deste passo você terá em `.debug/` tudo que o agente precisa para investigar sem precisar conhecer a estrutura do projeto.

### Passo 5: Acionar o Agente `debug-gdb` para Localizar a Causa Raiz
Invoque o agente `debug-gdb` (subagente genérico de depuração GDB, reutilizável em qualquer projeto 42) passando no prompt:
- **Contexto da função:** o que a(s) função(ões) envolvidas devem fazer (contrato esperado)
- **Cenário do teste:** o mesmo cenário extraído no Passo 2
- **Erro inicial do tester:** o erro/saída real capturado no Passo 1
- **A mesma análise do Passo 3:** o bloco "Análise da Falha do Teste" já apresentado ao usuário
- O caminho do sandbox montado no Passo 4 (`.debug/`)

O agente compila o sandbox, investiga com GDB e devolve: causa raiz, localização exata (função/arquivo/linha) e uma correção proposta em formato de diff. NÃO aplique nada ainda — o agente não tem (e não deve ter) permissão para editar os arquivos-fonte reais.

### Passo 6: Propor Correção como Diff
Repasse ao usuário o diff devolvido pelo agente, no mesmo formato estruturado (linhas antigas → novas) SEM aplicá-lo:
```
## Correção Proposta

**Arquivo:** [caminho]
**Função:** [nome]
**Mudança:**
- [linha(s) antiga(s) de código]
+ [linha(s) nova(s) de código]

**Motivo:** [explicação de uma frase de por que isso corrige o problema]
```

### Passo 7: Aguardar Confirmação Explícita
Pergunte ao usuário para confirmar o diff proposto. Opções:
- ✅ **Aprovar:** Aplique o fix e teste
- ❌ **Rejeitar:** Descarte e proponha alternativa
- 🔧 **Ajustar:** Solicite modificação do diff antes de aplicar

Aplique mudanças apenas após aprovação explícita do usuário.

### Passo 8: Testar o Fix e Limpar o Sandbox
Após aprovação:
1. Aplique o fix nos arquivos reais de `src` usando ferramenta Edit em modo manual (usuário vê diff) — nunca em `.debug/`
2. Re-execute testador desde Passo 1
3. Verifique se este teste agora passa
4. Se PASSOU: remova de `.debug/` os artefatos criados para este cenário (o `.c` de reprodução, o binário compilado e o arquivo de entrada copiado) — o sandbox é descartável por cenário e deve ficar limpo antes do próximo teste. Passe para próxima falha
5. Se FALHOU: mantenha o sandbox e retorne ao Passo 5, acionando o agente novamente com os novos dados de erro

### Passo 9: Passar para Próximo Teste
Uma vez que teste atual passa e o sandbox foi limpo, volte para Passo 1 para próxima falha. Nunca processe múltiplos fixes de teste em lote.

## Regras Principais
- **Um teste por vez:** Identifique, analise, corrija um teste antes de passar para próximo
- **`src` é a fonte de verdade:** toda correção definitiva é aplicada nos arquivos de `src`; `.debug/` é só sandbox de investigação
- **Delegue a investigação técnica ao agente `debug-gdb`:** ele recebe contexto da função, cenário, erro inicial e a análise do Passo 3, investiga com GDB e devolve causa raiz + diff — ele nunca edita arquivos reais
- **Sem mudanças de código sem revisão de diff:** Todas as edições usam ferramenta Edit; usuário deve aprovar diff antes de aplicar em `src`
- **Metadados primeiro:** Sempre extraia e apresente informações do teste antes de preparar o sandbox
- **Sandbox descartável:** monte `.debug/` do zero a cada teste e limpe-o assim que o teste correspondente passar
- **Sem suposições:** Se saída do teste, do sandbox ou do agente não está clara, re-execute ou peça esclarecimento ao usuário
