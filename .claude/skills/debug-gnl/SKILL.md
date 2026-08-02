---
name: debug-gnl
description: Depuração de falhas do testador GNL um teste por vez com saída estruturada e revisão de diff
---

# Skill de Depuração GNL

Esta skill depura falhas do testador GNL (Get Next Line) seguindo um framework rigoroso de um-teste-por-vez. Nunca processe múltiplas falhas de teste em uma iteração.

## Configuração
- **Localização do testador:** diretório `gnlTester/`
- **Comando de execução:** `wsl -e bash -c "cd /home/pauloh8755/projects/common-core/get-next-line/gnlTester && make 2>&1"`
- **Arquivos de código:** `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`
- **Arquivos de entrada de teste:** diretório `gnlTester/files/`

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

Aguarde o usuário confirmar antes de prosseguir para análise de código.

### Passo 4: Localizar Raiz da Causa
- Leia o(s) arquivo(s) de código-fonte relevante(s)
- Trace o fluxo de execução da localização do erro para trás
- Identifique falha lógica, erro off-by-one, problema de limite de buffer, etc.
- NÃO aplique correções ainda

### Passo 5: Propor Correção como Diff
Mostre o fix como um diff estruturado (linhas antigas → novas) SEM aplicá-lo:
```
## Correção Proposta

**Arquivo:** [caminho]
**Função:** [nome]
**Mudança:**
- [linha(s) antiga(s) de código]
+ [linha(s) nova(s) de código]

**Motivo:** [explicação de uma frase de por que isso corrige o problema]
```

### Passo 6: Aguardar Confirmação Explícita
Pergunte ao usuário para confirmar o diff proposto. Opções:
- ✅ **Aprovar:** Aplique o fix e teste
- ❌ **Rejeitar:** Descarte e proponha alternativa
- 🔧 **Ajustar:** Solicite modificação do diff antes de aplicar

Aplique mudanças apenas após aprovação explícita do usuário.

### Passo 7: Testar o Fix
Após aprovação:
1. Aplique o fix usando ferramenta Edit em modo manual (usuário vê diff)
2. Re-execute testador desde Passo 1
3. Verifique se este teste agora passa
4. Se PASSOU: Passe para próxima falha
5. Se FALHOU: Retorne para Passo 4 com novos dados de erro

### Passo 8: Passar para Próximo Teste
Uma vez que teste atual passa, volte para Passo 1 para próxima falha. Nunca processe múltiplos fixes de teste em lote.

## Regras Principais
- **Um teste por vez:** Identifique, analise, corrija um teste antes de passar para próximo
- **Sem mudanças de código sem revisão de diff:** Todas as edições usam ferramenta Edit; usuário deve aprovar diff
- **Metadados primeiro:** Sempre extraia e apresente informações do teste antes de ler código-fonte
- **Diff antes de aplicar:** Sempre mostre diff e aguarde confirmação antes de Edit
- **Sem suposições:** Se saída do teste não está clara, re-execute ou peça esclarecimento ao usuário
