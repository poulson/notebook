---
layout: post
title: Stanford Applied Math Seminar
date: 2014-11-05 20:00:00
edit: 2014-11-09 10:00:00
location: Stanford (Stanford, CA)
categories: talks
---

<center><img src="{{ site.baseurl }}/assets/KSubDNested.png"></center>

I was recently asked to fill in for the applied mathematics seminar and 
decided to give my talk on a research program for the scalable 
distributed-memory inversion of $$ \mathcal{H} $$-matrices with standard 
admissibility conditions for the third time. The talk begins with an overview
of what I believe to be the most important theorem for the field, the main 
result of Bebendorf and Hackbusch's 
[Existence of $$ \mathcal{H} $$-matrix approximants to the inverse FE-matrix of elliptic operators with $$ L^\infty $$-coefficients](http://link.springer.com/article/10.1007%2Fs00211-002-0445-6).

I had previously described the the proof of the theorem as constructively 
building the basis for the low-rank approximations of off-diagonal blocks of 
the Green's function via the recursive application of a mechanism which first 
applies Poincaré's inequality over a covering of the subdomain in order to 
provide a bound for the piecewise-constant approximation error relative to the 
$$ L^2 $$ norm of the gradient, followed by a Caccioppoli inequality so that 
the gradient can itself be bounded by the $$ L^2 $$ norm of the Green's function
over a neighborhood of the original subdomain (with the bound depending upon the
square-root of the eigenvalue contrast of the permeability matrix $$ K $$ from the weighted negative Laplacian $$ - \nabla \cdot K \nabla $$). In particular,
I had described the recursion as the reapplication of the 
Poincaré + Caccioppoli mechanism to the piecewise-constant approximation error 
with the purpose of exponentiating the error decay at the expense of only 
multiplying the dimension of the subspace that had been required from a single 
application of the mechanism.

*However*, just before my talk began, I realized that my previous two talks on 
the subject, one at MIT and one at KAUST, had glossed over a crucial piece of 
the theorem: in order for the recursion to be valid, the piecewise-constant 
approximation associated with the Poincaré bound must be replaced with its
$$ L^2 $$-projection onto the closed subspace of $$ L^2 $$ containing the 
restricted Green's function (in particular, functions harmonic with respect to
the "off-diagonal block" of the elliptic operator and within $$ H^1 $$ of 
every subdomain separated from the boundary). In retrospect, missing this 
crucial detail led me to make at least one false statement in my previous talks
on the subject.

The corrected talk, which had a rocky start due to the need for *extremely* 
last minute modifications, is available here: 
[Towards the scalable inversion of structured matrices with standard admissibility conditions]({{ site.baseurl }}/slides/Stanford-HMat-Nov5-2014.pdf). 

*Availability*: The source code for the prototype used to produce the
distributed-memory $$ \mathcal{H} $$-matrix composition results in the talk
is called [Distributed-Memory Hierarchical Matrices and is available on 
BitBucket](https://bitbucket.org/poulson/dmhm).
