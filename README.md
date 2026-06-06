<!--
 * EVF9 - Extended Vehicle Framework 9
 *
 * Created by EAST9
 * Open.MP / SA-MP Community
 *
 * GitHub: https://github.com/east9-777
 * Discord: leivison0230_58048
 * Comunidade Discord: https://discord.gg/FJBWAesQV8
 *
 * Copyright (c) EAST9
 * Do not remove this notice.
-->

# 🚗 EVF9 — EXTENDED VEHICLE FRAMEWORK 9
**EVF9** é uma plugin que mudar completamente a forma como você gerencia os veículos do seu servidor SA-MP ou Open.MP.
Diferente de tudo o que você já viu, ele não é um monte de includes pesadas em Pawn. O EVF9 é um **plugin desenvolvido em C++** trabalhando lado a lado com uma **API em Pawn**. Isso significa que todo o processamento bruto, loops e salvamento de dados rodam direto no plugin, garantindo um desempenho absurdo para o seu servidor.
[![Platform](https://img.shields.io/badge/platform-Open.MP%20%7C%20SA--MP-blue)](https://open.mp)
[![Version](https://img.shields.io/badge/version-1.0.0-green)](https://github.com/east9-777)
[![License](https://img.shields.io/badge/license-custom-orange)](LICENSE)

## ⚡ Recursos Principais (Features)
O framework transforma os carros em entidades completas e inteligentes com o melhor da engenharia de software:
 * **Estado Autoritativo:** Os dados ficam guardados direto na memória do plugin em C++. Chega de duplicar variáveis no Pawn e estourar a memória do servidor!
 * **IDs Permanentes (UIDs):** Cada veículo ganha um identificador único que nunca muda, mesmo que o carro dê respawn, mude de slot, exploda ou o servidor reinicie.
 * **Loops O(n) Inteligentes:** Chega de dar for(new i; i < MAX_VEHICLES; i++) e travar a CPU checando slots vazios! A macro EVF9_FOREACH_VEHICLE usa listas encadeadas (*Linked Lists*) para varrer apenas os veículos que realmente existem.
 * **Flags Bitwise:** Consultas e alterações de estado instantâneas usando operações binárias rápidas.
 * **Máquina de Salvamento Segura:** Controle inteligente de estados (*Dirty*, *Saving*, *Clean*) que permite salvamentos assíncronos sem risco de perder dados.
 * **Escalabilidade Monstra:** Desenvolvido para aguentar rodar liso de 500 a mais de 5.000 veículos ao mesmo tempo!

## 📦 Estrutura Totalmente Modular
O EVF9 é 100% independente. Você só ativa e inclui no seu código o que o seu modo de jogo for usar:

### 🔩 Módulos Nativos (Opcionais)
 * **evf9_ownership**: Controle de donos de veículos e transferência de propriedades.
 * **evf9_fuel**: Combustível dinâmico (0.0 a 100.0) com taxas de consumo customizáveis por carro.
 * **evf9_damage**: Mecânica avançada de reparos e dados detalhados de danos em painéis, portas e pneus.
 * **evf9_keys**: Sistema completo de trancar/destrancar e gerenciamento de chaves para os jogadores.
 * **evf9_fleet**: Agrupamento de veículos em frotas (perfeito para corporações e facções).
 * **evf9_tracking**: Rastreamento de distância percorrida e armazenamento da última posição do mapa.
 * **evf9_insurance**: Seguros de veículos controlados por tempo através de Timestamp Unix.

### 🔌 Extensões e Integrações Nativas
 * 💾 **Bancos de Dados:** Suporte nativo para MySQL, SQLite, JSON e Y_INI.
 * 🚀 **Integrações Externas:** Conexão direta com o plugin *Streamer* (para anexar objetos dinâmicos), *FCNPC* (para criar tráfego de bots) e webhooks/logs automáticos via HTTP direto para o seu **Discord**!

## 🧠 Sistema de Eventos Canceláveis e API
A API do EVF9 te dá superpoderes no código Pawn:
 * **Gatilhos de Ação:** Eventos cirúrgicos para o Ciclo de Vida do carro (criar, destruir, spawnar, abandonar), Motor (ligar, falhar, quebrar), Combustível (ficar baixo ou zerar), Danos (bater, explodir, capotar) e Segurança.
 * **Gatilhos Canceláveis:** Eventos como EVF9_OnVehicleTheftAttempt detectam nativamente quando alguém tenta roubar um veículo. Se você retornar EVF9_BREAK, o roubo é cancelado na hora!

## 🧩 Sistema de Addons (Crie suas próprias expansões!)
Quer criar novos sistemas para o EVF9 sem mexer no plugin principal? Você pode!
 * **Comunicação Limpa:** Crie includes secundárias que interagem apenas com a API pública, sem perigo de quebrar o plugin.
 * **Propriedades Customizadas:** Registre variáveis novas (como o nível de Nitro/NOS do veículo) direto na memória do C++!
 * **Exemplos Prontos:** O projeto já vem com templates para sistemas de corrida (evf9_racing), polícia (evf9_police) e empregos (evf9_jobs).

## 🍪 Exemplo de Código Limpo
```pawn
#include <EVF9>
#include <EVF9/modules/evf9_fuel>

public OnGameModeInit() {
    // Cria um veículo gerenciado pelo plugin
    new vehicleid = CreateManagedVehicle(411, 0.0, 0.0, 3.0, 0.0, 1, 1);
    
    // Indexa uma etiqueta e zera o combustível
    EVF9_SetVehicleTag(vehicleid, "Infernus de Teste");
    SetVehicleFuel(vehicleid, 0.0); 
    return 1;
}

// Interceptando o motor usando eventos canceláveis
public EVF9_OnVehicleEngineStart(vehicleid, playerid) {
    new Float:fuel;
    GetVehicleFuel(vehicleid, fuel);
    
    if(fuel <= 0.0) {
        SendClientMessage(playerid, -1, "❌ Sem combustível! O motor não vai ligar.");
        return EVF9_BREAK; // Bloqueia o motor de ligar nativamente!
    }
    return EVF9_CONTINUE;
}

```

## ⚙️ Performance de Ponta e Diagnósticos
 * **Cache Interno:** Posição, velocidade e ocupantes são cacheados no C++ e sincronizados no intervalo que você quiser (ex: EVF9_CACHE_INTERVAL 1000), poupando sua CPU.
 * **Profiler e Monitor de Memória:** Ative o profiler integrado para medir o tempo exato que suas funções levam para rodar e monitore o consumo de bytes do servidor.
 * **Anti-padrões:** O framework proíbe o uso de strings em loops quentes (como strcmp) — use os metadados inteiros do plugin para máxima velocidade!

## 🛠️ Resolução de Problemas (Troubleshooting) e Segurança
 * **Vida do veículo dessincronizada?** Não use o SetVehicleHealth padrão do SA-MP. Use sempre a nativa EVF9_SetVehicleHealth para atualizar o estado autoritativo do plugin.
 * **Carro não tranca?** Lembre-se que o evento EVF9_OnVehicleLock é cancelável. Se outra include retornar EVF9_BREAK, o carro continuará aberto.
 * **Seguros Inconsistentes?** Nunca force a flag VEHICLE_FLAG_INSURED manualmente. Use a função InsureVehicle para salvar o tempo Unix de expiração e a flag juntos.
 * **Salvamento Assíncrono sem Perda de Dados:** Se um veículo for deletado via código bem na hora de um salvamento no MySQL (*Race Condition*), o EVF9 adia a destruição física dele até que a função EVF9_EndSave termine. Zero perda de dados!
 * **Debug Ativo:** Ative o modo de depuração profunda definindo #define EVF9_ENABLE_DEBUG 1 antes da include para ver tudo no console.

## 🗺️ Roadmap de Desenvolvimento
 * 🏁 **v1.0 (Atual):** Sistema base, Iterador inteligente, Módulos principais e salvamentos.
 * 🚀 **v1.5:** Expansão de rastreamentos, frotas, seguros e sistema de addons.
 * 🤖 **v2.0:** Sistema inteligente de tráfego de NPCs, rotas com IA e economia de veículos.
 * ☁️ **v3.0:** Física avançada de veículos, sincronização em nuvem e um Dashboard via Web.
 
## 👑 REGRA DE OURO DO PROJETO
> ⚠️ **NUNCA use o vehicleid como chave primária no seu banco de dados!** Como os IDs de slots mudam a todo momento no SA-MP quando os carros dão respawn ou o servidor reinicia, use sempre o **EVF9_GetVehicleUID(vehicleid)**. Esse UID é permanente, exclusivo e gerado de forma segura pelo plugin em C++!
> 

## 📈 Especificações Técnicas
 * **Plataforma Primária:** Open.MP (64-bit / x86_64).
 * **Suporte Parcial:** SA-MP 0.3.7 (32-bit / x86 usando a flag --samp).
📄 **Repositório:** https://github.com/east9-777
👥 **Discord de Suporte:** https://discord.gg/FJBWAesQV8
👑 **Autor:** EAST9
