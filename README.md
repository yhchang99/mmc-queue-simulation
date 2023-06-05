# README

## Usage
The M/M/1 model is a fundamental queuing theory model that describes a single-server queue with Poisson arrivals and exponential service times. 

![the picture of the M/M/c model](https://github.com/yhchang99/Waiting-time-distribution-for-the-M-M-m-queue/blob/master/img/mmn.png)

- Arrival rate (λ): The average number of arrivals per unit of time.
```math
\lambda_n = \lambda
```
- Service rate (μ): The average number of customers served per unit of time.
```math
    \mu_n = 
    \begin{cases}
    n \mu, & \; (1 \leq n < c) \\
    c \mu, & \; (n \geq c)
    \end{cases}
```
- $p_n$ :
```math
    p_n =
    \begin{cases}
    \frac{\lambda^n}{n!\mu^n}p_0, (1 \leq n < c) \\
    \frac{\lambda^n}{c^{n-c}c!\mu^n}p_0, (n \geq c)
    \end{cases}
```
- Furthermore, we can calculate $p_0$
```math
    p_0 = \Big(\sum_{n=0}^{c-1}\frac{r^n}{n!} + \sum_{n=c}^{\infty}\frac{r^n}{c^{n-c}c!}\Big)^{-1}, \text{ where } r = \frac{\lambda}{\mu}, \rho = \frac{r}{c}
```
- Average number of customers in the system (L): 
```math
L = r + L_q = r + \frac{r^c \rho p_0}{c!(1-\rho)^2}
```
- The Waiting time in queue:
```math
    \begin{align}
    W_q(t) &= Pr\{T_q \leq 0\} \\
    &= \sum_{n=c}^{\infty} Pr\{n - c + 1 \text{ completions } \leq t |\text{arrival found n in system}\} \cdot p_n + W_q(0) \\
    &= \sum_{n=c}^{\infty} \frac{r^n}{c^{n-c}c!}p_0 \cdot \int_0^t \frac{c \mu (c \mu x)^{n-c}}{(n-c)!}e^{-c \mu x} dx + W_q(0) \\
    &= \frac{r^c p_0}{(c-1)!} \int_0^t \mu e^{-c \mu x}\sum_{n=c}^{\infty} \frac{(\mu r x)^{n-c}}{(n-c)!} dx + W_q(0) \\
    &= \frac{r^c p_0}{(c-1)!} \int_0^t \mu e^{-\mu x (c-r)} dx + W_q(0) \\
    &= \frac{r^c p_0}{(c-1)!(c-r)} \int_0^t \mu (c-r) e^{-\mu x (c-r)} dx + W_q(0) \\
    &= \frac{r^c p_0}{c!(1-\rho)} [1-e^{-\mu(c-r)t}] + W_q(0) \\
    &= \frac{r^c p_0}{c!(1-\rho)} [1-e^{-\mu(c-r)t}] + \Big(1 - \frac{r^c p_0}{c!(1-\rho)} \Big) \\
    &= 1 - \frac{r^c p_0}{c!(1-\rho)}e^{-(c \mu - \lambda)t}
    \end{align}
```

This project is avaliable on: https://github.com/yhchang99/Waiting-time-distribution-for-the-M-M-m-queue

## Installation
To compile and run the program, you must have a C compiler installed on your system, such as `GCC` or `Clang`.
Clone the repository to your local machine:

```bash
git clone https://github.com/yhchang99/Waiting-time-distribution-for-the-M-M-m-queue
```
Navigate to the project directory:
```bash
cd Waiting-time-distribution-for-the-M-M-c-queue
```

## Compile and run the program:

```bash
make
```

## Plot the results of the program
To plot the diagram, you must have `gnuplot` installed on your local machine:
```bash
make plot
```

## Example
Here's an example usage of the program:
```bash
*********Parameter*********
arrival_rate: 0.500000
service_rate: 1.000000
Number of servers: 1
Number of samples: 1000000
Number of simulation: 10

*********Theoretical*******
Mean of System  Time: 2.000000
Mean of Waiting Time: 1.000000

*********Simulated*********
Mean of System  Time: 2.002891
Mean of Waiting Time: 1.002525
```

![the pdf of the waiting time](https://github.com/yhchang99/Waiting-time-distribution-for-the-M-M-c-queue/blob/master/img/wait_time.png)

## License
MIT License
Copyright (c) 2023 yhchang99
