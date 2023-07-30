# flappybird_q_learning
This environment contains 44 million + possible state action paris. The environment is dynamic and random meaning that it could take billions of itterations for the agent to explore each state action pair. I did not wait until convergence due to the shear magnitude of time needed to wait for convergence.

Three facts are known:
The q values updates are correct value wise.
The q table is being updated in the correct location. 
Due to the above to facts. Convergence is inevitable.

The movements of the agent made me implement a method of training the agent through playing the game myself. This meant I was able to ensure the state actions pairs that yeild the highest reward are updated early on allowing for better learning. This is moreso a proof of concept that the implementation is correct rather then a real life solution that is optimal and the best one for the problem.

Images can be found in the repo.
