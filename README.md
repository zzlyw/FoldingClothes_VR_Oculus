# VR Project for Folding Clothes

This repo introduces the UE4 code for our IROS 2020 paper.  

**Graph-based Hierarchical Knowledge Representation for Robot Task Transfer from Virtual to Physical World**

Zhenliang Zhang, Yixin Zhu, Song-Chun Zhu  
To appear in *Proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS)*, 2020.  

We study the hierarchical knowledge transfer problem using a cloth-folding task, wherein the agent is first given a set of human demonstrations in the virtual world using an Oculus Headset, and later transferred and validated on a physical Baxter robot. We argue that such an intricate robot task transfer across different embodiments is only realizable if an abstract and hierarchical knowledge representation is formed to facilitate the process, in contrast to prior literature of sim2real in a reinforcement learning setting. Specifically, the knowledge in both the virtual and physical worlds are measured by information entropy built on top of a graph-based representation, so that the problem of task transfer becomes the minimization of the relative entropy between the two worlds. An And-Or-Graph (AOG) is introduced to represent the knowledge, induced from the human demonstrations performed across six virtual scenarios inside the Virtual Reality (VR). During the transfer, the success of a physical Baxter robot platform across all six tasks demonstrates the efficacy of the graph-based hierarchical knowledge representation. 


# Requirements

**Hardware**
* Oculus Rift CV1

**Software**
* Unreal Engine 4.18 on Windows 10


# Usage


1. Download the Unreal Engine from here: https://www.unrealengine.com, and UE4.18 is recommended.

2. Setup UE4 on your windows 10 computer.

3. Download the files in this repo, and put the files to a new empty UE4 project.

4. Setup the Oculus Rift. Then open the VR device support in UE4 and compile the project.






