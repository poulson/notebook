---
layout: page
title: Artifacts
permalink: /artifacts/
---

The scholarly products of my research tend to take the form of
openly-developed distributed-memory math libraries coupled with one or more 
journal publication. 

Weighed by my invested time, my most significant 
contribution is almost certainly 
<a href="http://libelemental.org">Elemental</a>, which builds up from
<a href="http://netlib.org/blas">BLAS</a>/<a href="http://netlib.org/lapack">LAPACK</a>/<a href="http://en.wikipedia.org/wiki/Message_Passing_Interface">MPI</a> in order to provide C++/C/Python bindings for distributed-memory dense and 
sparse-direct linear algebra and optimization. The most recent additions are <a href="http://libelemental.org/documentation/dev/lapack_like/spectral/Pseudospectra.html">novel routines for the high-performance computation of pseudospectra</a> and <a href="http://libelemental.org/documentation/dev/optimization.html">distributed-memory sparse Interior Point Methods for linear and quadratic programs</a>. Publications for these two recent additions will follow the release of version 0.86 of the library.

I have also dedicated a substantial amount of time towards the development of <a href="https://github.com/poulson/dist-butterfly">a distributed-memory butterfly algorithm for Egorov-like operators on uniform domains</a>, <a href="https://github.com/poulson/PSP">distributed-memory sweeping preconditioners</a>, and <a href="https://bitbucket.org/poulson/dmhm">the distributed-memory inversion of H-matrices with standard low-rank admissiblity conditions</a>. The outlier in this
list is that my work on distributed-memory H-matrices has not yet been submitted
for publication due to the immense gulf that is sometimes present between 
asymptotically-fast and practically-fast algorithms.
