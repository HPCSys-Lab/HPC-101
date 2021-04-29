# Pi

This program calculates pi using an infinite series approach.

From trigonometry, we have:

`tan(pi / 4) = 1`

Using the inverse tangent function, `arctan(x)`, we have:

`arctan(1) = pi / 4`

So, we can write:

`pi = arctan(1) * 4`

Now, we need to calculate `arctan(1)`. 

Using a method known as the Gregory-Leibniz Series or the Madhava–Gregory series, we have the following formula to calculate `arctan(x)`:

![alt text](https://github.com/HPCSys-Lab/HPC-101/blob/main/img/pi_arctan.png)

Where `n` is the number of terms of the series (or number of iterations) and `x = 1`.

