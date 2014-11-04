---
layout: post
title: Trip to Extreme Computing Research Center
date: 2014-11-01 13:40:00
edit: 2014-11-04 11:27:00
location: KAUST (Thuwal, Saudi Arabia)
categories: talks
---

![KAUST's beacon]({{ site.baseurl }}/assets/KAUST-beacon.png)

It was a pleasure to directly fly out to David Keyes' 
[Extreme Research Computing Center](http://ecrc.kaust.edu.sa/Pages/Home.aspx) 
from my visit to the 
[Innovative Computing Laboratory](http://www.icl.utk.edu/about) (indeed, there
are several researchers who have been affiliated with both groups, such as 
[Hatem Lateif](http://ecrc.kaust.edu.sa/Pages/People-Ltaief.aspx)).
A number of the students, such as 
[Gustavo Chavez Chavez](http://ecrc.kaust.edu.sa/Pages/People-Chavez.aspx) 
are working on structured matrices, and several researchers, such as 
[Rio Yakota](http://ecrc.kaust.edu.sa/Pages/People-Yokota.aspx) 
(of [ExaFMM](https://bitbucket.org/rioyokota/exafmm-dev) fame), are actively
developing algorithms and software for FMM.

On my second day visiting the group, I gave a seminar summarizing my research
program focused on developing [a scalable algorithm for inverting 
$$ \mathcal{H} $$-matrices with standard admissibility conditions on 
distributed-memory architectures]({{ site.baseurl }}/slides/KAUST-HMat-Oct28-2014.pdf). In particular, the main idea is to convert a weakly-admissible 
$$ \mathcal{H}^2 $$ (aka, HSS) ULV factorization 
(which will require a higher rank than with standard admissiblity, but is 
highly parallelizable) into a direct inverse via a "peeling" algorithm, to then
convert the HSS inverse into a standard admissiblity $$ \mathcal{H} $$-matrix
(with lower rank), and then to run a highly parallel Newton-Schulz procedure
to achieve a higher-quality inverse with a modest rank. Results were presented
for the distributed-memory $$ \mathcal{H} $$-matrix composition portion.
