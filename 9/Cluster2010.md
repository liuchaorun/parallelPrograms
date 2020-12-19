## Adaptive Optimization for Petascale Heterogeneous CPU/GPU Computing

### 摘要

在本文中，我们描述了为天河1开发Linpack基准测试的实验，该系统是Petascale（千万亿次） CPU/GPU超级计算机系统，这是迄今为止尝试过的最大的GPU加速系统。本文提出了一种自适应优化框架，以平衡GPU和CPU上的工作负载分配与可忽略的运行时开销，从而获得比静态或训练分区方法更好的性能。通过软件流水线技术可以有效地隐藏CPU-GPU通信开销，这对于大型内存绑定应用程序特别有用。结合其他传统优化，我们使用自适应优化框架进行优化的Linpack在TianHe-1的单个计算元素上实现了196.7GFLOPS。此结果是峰值计算能力的70.1％，比使用供应商的库的结果快3.3倍。在TianHe-1的完整配置上，我们的优化使Linpack的性能达到0.563 PFLOPS，这使TianHe-1成为2009年11月发布的Top500排行榜中排名第五的超级计算机。

### 介绍

超级计算机进入千万亿级领域。
