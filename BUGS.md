BUGS
====

List of known bugs.

If you know a bug that is not here, add it!

Currently Open Bugs
--------------------

A RESPOSTA DESSAS DUAS ACHO Q EH PQ MOVE_TABLE NEM SEMPRE EH FICAR PARADO
- Mesmo com o oponente apenas com a opcao de move_table (ficar parado) a best_action do minmax define uma posicao que não é a dele (ficar parado).
- Quando jogo todos os oponentes para fora, embora na visao (entrada pro minmax) os oponentes estejam fora o move do ultimo a ser removido cai dentro do campo, pq?

- DriveToBall ainda está demorando para fazer a voltinha e as vezes ele trava.
- Pq quando ele tá longe da bola as vezes ele prefere um move do que um get_ball ????
- No minmax robôs ficam saindo do campo, pq?
- Condição para gerar estabilidade no minmax ainda não está legal no AttackerMinMax2
- Rotina de pesar por alterar move do minmax não está funcionando (acho q melhor não é pesar por alterar e sim por gerar instabilidade).
- Melhorar a arquitetura do LinearCombMotion e DriveToball, pois são skills contendo skills, o que dificulta o uso do setRobot.
- Finalizar o GotoAvoid.
- Robô não deve chutar enquanto o dribble estiver e a bola estiver na inercial do giro do driblador.
- Minmax não segue as regras do jogo(comando do juiz e não colidir)
- Zicker43 só chuta no meio do gol.
- Minmax não chuta no meio do maior buraco.
- Goto demora para atingir a posição final, ficando lento quando
  está chegando perto (no Goto poderia ser passado a velocidade final desejada), e fazer um controle conceitualmente correto.
- Controller2 o robô não está mais olhando fixamente para bola.
- SampledKick::calculatePassPower não funciona para um vFin genérico.
- Simulador está descasado do campo/robô/bola real.

