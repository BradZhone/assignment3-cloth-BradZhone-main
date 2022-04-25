# **Assignment 3 Cloth Simulation with the Mass Spring System**

---

## General Guidance

1. **Plagiarism is always strictly forbidden**
2. These assignments are designed to help you to learn the algorithms and meanwhile practice your programming skills. **We may or may not provide reference resources and everything is up to your design**
3. Various implementations are accepted as long as the results are satisfactory. You can use your favorite language or frameworks if it is not specified in assignment
4. **It is always requested to explain your codes to our TA after submission**
5. It is encouraged to utilize open source projects or third party libraries for **non-core** part (tagged with **[non-core]**) of your homework **BUT you are requested to also explain the key part of how their codes work**. Usage of misunderstood or mysterious codes leads to a score deduction
6. You need implement yourself for the **core** part (tagged with **[core]**)
7. Feel free to send email to TA or post question on Piazza for help
8. Good luck and have fun

## Introduction

In assignment 3, you are expected to use C++ with OpenGL to implement a simple mass spring solver to realize a simple cloth simulation from scratch. For this assignment, we are providing some reference codes to show you a simple OpenGL framework with an example rectangular piece of cloth.

## Reading Materials

The implementation would become easier if you understand the whole process before you start. There are some materials which might be helpful. Have a quick look before you start.

1. You can play with a toy demo of a cloth simulation with mass spring [here](https://jklintan.github.io/Cloth-Simulation/).
2. You can play with another toy demo of a mass spring [here](https://www.physicsclassroom.com/Physics-Interactives/Waves-and-Sound/Mass-on-a-Spring/Mass-on-a-Spring-Interactive) if you feel confused with stiffness and damping.
3. [Here](https://graphics.stanford.edu/~mdfisher/cloth.html), [here](https://www.ics.uci.edu/~shz/courses/cs114/docs/proj3/index.html) and [here](https://steven.codes/blog/cloth-simulation/) provide some good references about the cloth simulation with the mass spring system.
4. And always, you can refer to our course [slides](https://faculty.sist.shanghaitech.edu.cn/faculty/liuxp/course/cs275/) and [recordings](https://pan.shanghaitech.edu.cn/cloudservice/outerLink/decode?c3Vnb24xNjQ3MzM1ODI4OTY5c3Vnb24=) about the cloth simulation.

## Requirements

1. **[core]** Initialize particles of the mass spring system
   1. Input: parameters for a piece of rectangular cloth
   2. Hint: you should decide which physical properties of the particles should be saved
   3. Hint: the mass of the particles should sum up to the mass of the cloth
2. **[core]** Design the spring to connect two particles
   1. Hint: consider the data you need for the spring and its force
3. **[core]** Build the Mass Spring System to form the cloth
   1. Input: particles you build in step 1
   2. Algorithm: connect particles with strings in the system
   3. Hint: refer to the paper [here](http://kucg.korea.ac.kr/education/2005/CSCE352/paper/provot95.pdf) about the mass spring model to simulate the cloth.
4. **[core]** Implement the solver for the Mass Spring System
   1. Input: states of all particles and springs
   2. Algorithm: compute the forces at all particles and integrate them over time to update the position of the particles
   3. Hint: the update logic is similar to the last assignment
   4. Hint: either a semi-implicit euler integrator or a velocity-verlet integrator works good enough
   5. Hint: you can also choose to implement an implicit solver by solving a global linear system referred to the paper [here](http://tiantianliu.cn/papers/liu13fast/liu13fast.html) **[optional]**
5. **[core]** Add constraint to the position of the particle
   1. Hint: you can either fix the position after updating or give the particle a large mass
6. **[core]** Add damping to the mass spring system.
   1. Hint: the particle forces should be damped proportional to its velocity
7. **[core]** Build your simulator of the whole pipeline with the mass spring system to simulate the cloth
8. **[non-core]** Visualization System

## Checkpoints

1. **[Req. 7]** Visualize the cloth without any dynamics *[5 pts]*
2. **[Req. 1, 2, 3, 4]** Test your mass spring solver without gravity by adjusting the resting distance a little smaller/larger than initialization *[30 pts]*
3. **[Req. 5, 6]** Simulate the behavior of a piece of rectangular cloth under the gravity where left up and right up corner is fixed. *[50 pts]*
4. In the simulation, the cloth is stopped and remain stable finally *[15 pts]*
5. **[optional]** Apply external forces to the cloth to simulate a behavior of the wind *[10 pts]*
6. **[optional]** Add a sphere collider to simulate a piece of cloth falling on a sphere *[10 pts]*
7. **[optional]** The time step of each update can get no less than 0.01s under the gravity of -9.8m/(s^2) *[10 pts]*

## Starter Code and Submission

This semester, we take advantage of the github classroom to send and collect assignments. You can access [this link](https://classroom.github.com/a/u1gRr2Xg) to accept the repository with the reference codes and example models we provide. **Please clone the repository down** to write your homework and **submit with `git commit` and `git push`.** If you are not familiar with git or github, please take a look at [the github document](https://docs.github.com/en/get-started/using-git/about-git).

If you decide to start from scratch and use little reference resources, you can put your work in the `submission` folder. Otherwise, you can modify and add codes in the `reference` folder and commit directly.

Submission Deadline: <span style="color:red">**23:59 on May 12th 2022**</span>

---

ShanghaiTech CS275 2022 Spring
