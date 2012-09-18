BUGS
====

List of known bugs.

If you know a bug that is not here, add it!

Currently Open Bugs
--------------------
- no gotoavoid para evitar ficar travando na hora de fazer a volta, a melhor solucao seria usar dois circulos concetricos para definir o estado de transicao.
- move_table é incompleta, pois não tenho todas as informações necessárias para executar a action anterior (só tem posicao)

A RESPOSTA DESSAS DUAS ACHO Q EH PQ MOVE_TABLE NEM SEMPRE EH FICAR PARADO
- Mesmo com o oponente apenas com a opcao de move_table (ficar parado) a best_action do minmax define uma posicao que não é a dele (ficar parado).
- Quando jogo todos os oponentes para fora, embora na visao (entrada pro minmax) os oponentes estejam fora o move do ultimo a ser removido cai dentro do campo, pq?

- Pq quando ele tá longe da bola as vezes ele prefere um move do que um get_ball ????
- No minmax robôs ficam saindo do campo, pq?
- Minmax não chuta no meio do maior buraco.
- Condição para gerar estabilidade no minmax ainda não está legal no AttackerMinMax2
- pesar na escolha do minmax em funçao da variacao do angulo de destino e não a variacao da distancia do ponto de destino

- Minmax não segue as regras do jogo(comando do juiz e não colidir)
- Minmax tem q receber qual eh o goleiro para soh ele poder querer entrar na area.
- Minmax conduz a bola acima do permitido.

- DriveToBall ainda está demorando para fazer a voltinha e as vezes ele trava (testar no real).
- Finalizar o GotoAvoid.
- Zicker43 só chuta no meio do gol.
- Melhorar a arquitetura do LinearCombMotion e DriveToball, pois são skills contendo skills, o que dificulta o uso do setRobot.
- Robô não deve chutar enquanto o dribble estiver e a bola estiver na inercial do giro do driblador.
- Goto demora para atingir a posição final, ficando lento quando
  está chegando perto (no Goto poderia ser passado a velocidade final desejada), e fazer um controle conceitualmente correto.
- Controller2 o robô não está mais olhando fixamente para bola.
- SampledKick::calculatePassPower não funciona para um vFin genérico.
- Simulador está descasado do campo/robô/bola real.

