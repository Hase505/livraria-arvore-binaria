## Pull Request Review Checklist

### 1. Objetivo e Clareza
- [ ] O que foi feito está claro
- [ ] A motivação da mudança foi explicada
- [ ] Há ligação com uma issue ou tarefa

---

### 2. Design e Qualidade do Código
- [ ] O código está legível e bem estruturado
- [ ] O princípio de responsabilidade única está sendo respeitado
- [ ] Não há duplicação de lógica que poderia ser extraída
- [ ] Uso apropriado de ponteiros, structs e `goto` (se aplicável)

---

### 3. Segurança e Robustez
- [ ] Há validação de entradas (ex: `NULL`, limites)
- [ ] Recursos alocados (memória, arquivos, etc.) são corretamente liberados
- [ ] Retornos de funções sensíveis são tratados corretamente
- [ ] Nenhum uso inseguro de memória identificado

---

### 4. Estilo e Convenções
- [ ] Nomenclatura e estilo seguem os padrões do projeto
- [ ] Formatação está consistente (indentação, espaçamento, etc.)
- [ ] Não há código morto, prints de debug ou comentários desnecessários

---

### 5. Testes
- [ ] Há testes para todas as novas funções e fluxos
- [ ] Os testes cobrem casos de erro e de fronteira
- [ ] Testes são independentes e idempotentes
- [ ] Todos os testes passam

---

### 6. Valgrind / Verificação de Memória
- [ ] O código foi testado com Valgrind
- [ ] Nenhum vazamento de memória relatado
- [ ] Nenhum acesso inválido ou uso após `free()`

---

### 7. Outros
- [ ] Arquivos temporários ou gerados (ex: `*.o`, `*.swp`, `a.out`) **não** foram adicionados
- [ ] Mudança é coesa e não mistura várias features
- [ ] Documentação foi atualizada (se necessário)
