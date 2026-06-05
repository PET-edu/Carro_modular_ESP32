## 1. Introdução: O que é Arduino?

* Conceito básico de **Arduino**

  * Plataforma de prototipagem eletrônica
  * Baseada em **microcontrolador**
  * Muito usada em educação, automação e projetos makers
* Ideia central:

  * Facilitar o controle de sensores, atuadores e módulos
* Observação importante:

  * Presume-se que os alunos **já tenham um contato básico**, mas vale reforçar o conceito geral

---

## 2. Existem outros microcontroladores além do Arduino?

* Sim, existem **vários outros microcontroladores**
* Exemplo que será usado no curso/projeto:

  * **ESP32**
* Transição didática:

  * “Se o Arduino já faz tanta coisa, por que usar o ESP32?”

---

## 3. Arduino × ESP32: diferenças principais

### 3.1 Capacidade e recursos

* ESP32 é **mais potente** que o Arduino tradicional (ex: Uno)
* Possui:

  * Mais memória
  * Mais processamento
  * Mais periféricos internos

### 3.2 Linguagem e programação

* Arduino usa **C/C++ (Sketch)**
* ESP32:

  * Também usa **C/C++ (Arduino Framework)**
  * Pode usar outras linguagens e frameworks
  * Ele permite **aplicações mais complexas**, inclusive servidores web

---

## 4. Um diferencial importante do ESP32: página web embarcada

* O ESP32 pode:

  * Criar um **servidor web**
  * Hospedar uma **página HTML**
* Isso permite:

  * Controlar o dispositivo pelo navegador
  * Acessar via celular ou computador
* Exemplo prático:

  * Página web controlando pinos, LEDs ou motores

---

## 5. Diferença de tensão: 5V (Arduino) × 3.3V (ESP32)

### 5.1 Tensão de operação

* Arduino:

  * Pinos digitais trabalham em **5V**
* ESP32:

  * Pinos trabalham em **3.3V**

### 5.2 Isso é um problema?

* Não é um problema, é apenas uma **característica**
* Analogia didática:

  * Assim como um aparelho 110V queimar se ligado em 220V
  * Um módulo 5V pode não funcionar se ligado direto em 3.3V ou ser danificado se for feito o contrario

---

## 6. Compatibilidade de módulos e cuidados

* Nem todo módulo feito para Arduino funciona direto no ESP32
* É necessário:

  * Ver o **datasheet**
  * Conferir a **tensão lógica**
* Alguns dispositivos:

  * Funcionam tanto em **3.3V quanto em 5V**
* Outros:

  * Precisam de **adaptação de nível lógico**

---

## 7. Como fazer adaptação de tensão?

### 7.1 Conceito básico de transistores

* Transistor funciona como:

  * Um **interruptor controlado**
* Ideia didática:

  * Um sinal pequeno controla uma energia maior
* Importante:

  * O microcontrolador **não fornece corrente suficiente** para cargas grandes

---

## 8. Por que não ligar motores direto no Arduino ou ESP32?

* Motores:

  * Consomem **muita corrente**
* Problema:

  * Queima do microcontrolador
* Solução:

  * Usar um módulo intermediário

---

## 9. Ponte H: controle de motores

* Ponte H é:

  * Um conjunto de **transistores**
* Funções:

  * Ligar e desligar motores
  * Controlar a **direção de rotação**
* Vantagem:

  * Protege o microcontrolador
  * Permite controlar motores com segurança

---

## 10. Teste inicial da Ponte H (sem código)

* Montagem básica:

  * Fonte
  * Ponte H
  * Motor
* Teste:

  * Verificar se o motor gira
  * Conferir sentido de rotação
* Objetivo:

  * Validar o hardware antes do software

---

## 11. Comunicação via página web (primeira abordagem)

* Criação de uma página web no ESP32
* A página:

  * Envia comandos
* O ESP32:

  * Recebe comandos pela rede
  * Repassa para a lógica interna (porta serial ou GPIO)
* Demonstração prática:

  * Mostrar como a comunicação funciona

---

## 12. Encerramento da aula e transição

* O que foi visto:

  * Conceitos
  * Hardware
  * Comunicação web
* Preparação para a próxima aula


---

## 13. Segunda Aula: Controle prático com ESP32 e Ponte H

* Contexto inicial:

  * Já existe compreensão sobre:

    * ESP32
    * Página web embarcada
    * Conceito de controle remoto via rede
* Objetivo da aula:

  * Passar do **conceito** para o **controle real de hardware**

---

## 14. Controle direto da Ponte H via página web

* Nova proposta:

  * Em vez de apenas comunicação serial
  * A página web irá **controlar diretamente a Ponte H**
* Fluxo geral:

  * Página web → ESP32 → Ponte H → Motores
* Resultado esperado:

  * Controle de direção e movimento dos motores via navegador

---

## 15. Integração: web + código + hardware

* Papel do código:

  * Interpretar os comandos da página web
  * Acionar corretamente os pinos ligados à Ponte H
* Papel do hardware:

  * Receber os sinais e converter em movimento físico
* Importante:

  * Código e circuito devem estar alinhados

---

## 16. Montagem do carrinho robótico (estrutura modular)

* Com o controle funcionando:

  * Montagem física do carrinho
* Uso de um aparato modular:

  * Motores
  * Chassi
  * Rodas
* Liberdade de projeto:

  * Cada grupo pode montar de uma forma diferente
  * Desde que funcione corretamente

---

## 17. Personalização da página web de controle

* Ajustes visuais:

  * Botões
  * Layout
  * Cores
  * Tema
* Ajustes funcionais:

  * Tamanho dos botões
  * Organização dos controles
* Objetivo:

  * Tornar a interface **intuitiva e personalizada**

---

## 18. Onde conseguir controles e interfaces prontas?

* Fontes possíveis:

  * Sites
  * Repositórios
  * Exemplos online
  * IAs
* Importante:

  * Entender que o código pode ser:

    * Adaptado
    * Modificado
    * Personalizado
* Enfoque didático:

  * Aprender a **ler, entender e adaptar**, não apenas copiar

---

## 19. Adaptação de controles externos

* Possibilidades avançadas:

  * Uso de recursos do celular

    * Inclinação
    * Movimento
    * Toques
* Exemplo:

  * Giro do celular → comando de direção do carrinho
* Conceito central:

  * Converter um evento em um comando físico

---

## 20. Comunicação em rede

* Formas de acesso ao carrinho:

  * Pela rede local
  * Por um endereço gerado pelo ESP32
* Questões importantes:

  * Quem pode acessar?
  * Quantos dispositivos podem controlar?
* Observação:

  * O próprio ESP32 pode:

    * Gerar o sinal
    * Hospedar a interface
    * Receber comandos

---

## 21. Ajustes finais e correções

* Verificações práticas:

  * Motores invertidos
  * Sentido de rotação
  * Resposta aos comandos
* Correções possíveis:

  * No código
  * Na fiação
  * Na interface web

---

## 22. Revisão do sistema completo

* Conferir:

  * Conexões elétricas
  * Lógica dos botões
  * Estabilidade da comunicação
* Avaliar:

  * Se algo precisa ser reforçado ou refeito

---

## 23. Acabamento e experiência do usuário

* Refinamentos:

  * Tamanho dos botões
  * Cores e contraste
  * Organização da interface
* Experiência:

  * Facilidade de uso
  * Clareza dos comandos

---

## 24. Compartilhamento e uso em rede

* Possibilidades:

  * Compartilhar o acesso ao carrinho
  * Testar múltiplos usuários
* Discussão:

  * Limitações
  * Segurança básica
  * Controle de acesso

---

## 25. Encerramento da segunda aula

* O que foi alcançado:

  * Controle real de motores via web
  * Integração completa:

    * ESP32
    * Ponte H
    * Interface web
* Preparação para próximas etapas:

  * Expansões
  * Sensores
  * Automação mais avançada

---


