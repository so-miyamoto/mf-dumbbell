
# Dependency
- yaml-cpp

# How to use
~~~bash
make
~~~
generate the binary file of dumbbell model a.out
~~~bash
./a.out in.yml > log.dat
~~~
see log.dat
- velocity gradient (xx xy xz yx yy yz zx zy zz)
- stress (xx yy zz xy yz zx)
- time derivative of stress (xx yy zz xy yz zx)

# Dumbbell model

Variables
$r$: bond vector of a dumbbell.
$\zeta$ friction coefficient
$k$: spring constant
$\tau=\zeta/4k$: relaxation time
$\ell=\sqrt{3k_\mathrm{B}T/k}$: thermal equilibration length
$\kappa$: velocity gradient
$G=\nu k_\mathrm{B}T$: relaxation modulus
$\eta=G\tau$: viscosity
$\sigma$: stress

The dynamics equation is written as
$$
\frac{dr}{dt} = \kappa\cdot r-\frac{2k}{\zeta}+\sqrt{\frac{4k_\mathrm{B}T}{\zeta}}w(t)
$$
where $\langle w(t)\rangle=0$, $\langle w(t)w(t')\rangle=\delta(t-t')I$.

Equally,
$$
\frac{dr}{dt} = \kappa\cdot r-\frac{1}{2\tau}+\sqrt{\frac{\ell^2}{3\tau}}w(t)
$$

The stress is
$$
\sigma = \nu \langle F r\rangle = \nu k\langle r r\rangle=3G\langle rr\rangle/\ell^2
$$
and the deviatoric stress is 
$$
\sigma = G(3\langle rr\rangle/\ell^2-I)
$$
