# EVAA

Here you have the idea of EVAA: Ecological Vertical Agriculture Assistant. The project is
about building a vertical farm with the environment (i.e. lighting, water, fertilizer) controlled
by AI.
The project goal is to teach AI how to grow plants while preserving energy, water, and other
resources. It could be the next step to fix the world’s hunger problem.

# High-Level Project Description
The project aims to solve the problem of the lack of intelligent and scalable solutions for
efficient plant cultivation in limited space. It will approach the problem by optimizing
resource utilization to maximize plant growth. The proposed solution will employ machine
learning techniques, cheap electronic modules and widely accessible hardware elements to
reach this goal. The FPGA will be used to control mechanical parts, perform visual control
of plant growth and optimize the machine learning model used to control the environment.

The FPGA will utilize the reinforcement learning approach (RL). RL is one of the basic
machine learning paradigms. An RL algorithm learns how to achieve a specific goal by
interacting with an environment. It is rewarded when its decisions bring the goal closer or
penalized when the goal moves away. Each plant species may have different requirements
from the environment of its growth. Determining the way of influencing the environment
depending on the changing conditions seems to be the right task for RL. Electronic modules
and hardware components can be used to control supplies of water, nutrients, carbon
dioxide, insolation, humidity or temperature. Plant volume, height, mass or amount of
green area can be used to reward or penalize the algorithm. RL algorithm may need lots of
input data before it starts to know how to control the environment. In order to reduce the
complexity of the learning process, it's possible to develop a plan growth model and use it
as an initial source of data for the algorithm. Then, the knowledge from the 
simulation can be transferred to the model trained on real plants.