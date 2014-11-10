---
layout: post
title: A simple distributed algorithm for sparse-direct least squares
date: 2014-11-01 13:40:00
edit: 2014-11-03 14:33:00
location: KAUST (Thuwal, Saudi Arabia)
author:
    twitter: libelemental
categories: notes
---

On shared-memory architectures, sparse-direct QR factorizations are now a 
standard tool (see, for example, the work of Tim Davis or Alfredo Buttari), but
no distributed-memory implementations are publicly (or, I believe, privately) 
available. Thus, if one is tasked with solving very large sparse least squares
problems on distributed-memory architectures and desires a direct method, then
it is prudent to consider other possibilities.

Perhaps the simplest alternative involves recognizing that, if a sparse matrix 
$$ A \in \mathbb{C}^{m \times n} $$ has full rank, with 
$$ m \ge n $$, then the psuedoinverse is given by

$$ (A^H A)^{-1} A^H, $$

and so a sparse-direct Cholesky factorization of $$ A^H A $$, say 
$$ A^H A = R^H R $$, yields a (fast) direct method for applying the 
pseudoinverse. Thankfully, scalable implementations of distributed-memory 
sparse-direct Cholesky factorizations have existed in the public domain for
some time, and so the only extra requirement is a distributed-memory 
algorithm for forming $$ A^H A $$.

The simplest, and most common, distribution scheme for sparse matrices is to
partition the rows of the matrix in a contiguous manner among the processes.
That is to say, each entry $$ (i,j) $$ is mapped to process $$ q $$ via a 
mapping

   $$ d : i \mapsto q $$

and, typically, the preimage $$ d^{-1}(q) $$ is contiguous.

Given such a distribution scheme, it turns out to be significantly simpler to
form $$ A^H A $$ in parallel than for a general sparse matrix composition, 
say $$ A B $$. The reason is that entry $$ (i,j) $$ of the result is given
by:

$$ (A^H A)_{i,j} = \sum_k \overline{A(k,i)} A(k,j), $$

and, our one-dimensional distribution scheme guarantees that $$ A(k,i) $$ 
and $$ A(k,j) $$ are always assigned to the same process. Thus, each process
can locally compute its updates to the product and then participate in a single 
global AllToAll communication in order to send each update 
$$ \overline{A(k,i)} A(k,j) $$ to process $$ d(i) $$. 
If the maximum edge degree of the matrix is $$ n_e $$, and the maximum number 
of rows assigned to any process is $$ m_l $$,
it is easy to see that the incoming and outgoing per-process communication 
volumes are each bounded by $$ m_l n_e^2 $$, which is only a factor of $n_e$ 
times the worst-case amount of data locally assigned to any process.

Once $$ A^H A $$ has been formed, a distributed-memory sparse-direct Cholesky
factorization/solve (e.g., the one implemented within Elemental) can be applied
to a vector $$ A^H y $$ in order to solve a least-squares problem 
$$ \min_x \| A x - y \|_2 $$.

An example usage of the recently-added python wrappers to Elemental's 
distributed-memory sparse-direct least squares solver to solve a linear system
involving the vertical concatenation of a discrete 2D Laplacian with an 
identity matrix follows:

{% highlight python %}
import El
n0 = n1 = 20

def ExtendedLaplacian(xSize,ySize):
   A = El.DistSparseMatrix()
  A.Resize(2*xSize*ySize,xSize*ySize)
  firstLocalRow = A.FirstLocalRow()
  localHeight = A.LocalHeight()
  A.Reserve(5*localHeight)
  hxInvSq = (1.*(xSize+1))**2
  hyInvSq = (1.*(ySize+1))**2
  for sLoc in xrange(localHeight):
    s = firstLocalRow + sLoc
    if s < xSize*ySize:
      x = s % xSize
      y = s / xSize
      A.QueueLocalUpdate( sLoc, s, 2*(hxInvSq+hyInvSq) )
      if x != 0:       A.QueueLocalUpdate( sLoc, s-1,     -hxInvSq )
      if x != xSize-1: A.QueueLocalUpdate( sLoc, s+1,     -hxInvSq )
      if y != 0:       A.QueueLocalUpdate( sLoc, s-xSize, -hyInvSq )
      if y != ySize-1: A.QueueLocalUpdate( sLoc, s+xSize, -hyInvSq )
    else:
      A.QueueLocalUpdate( sLoc, s-xSize*ySize, 2*(hxInvSq+hyInvSq) )

  A.MakeConsistent()
  return A

A = ExtendedLaplacian(n0,n1)
El.Display( A, "A" )
El.Display( A.DistGraph(), "Graph of A" )

y = El.DistMultiVec()
El.Uniform( y, 2*n0*n1, 1 )
El.Display( y, "y" )
rank = El.mpi.WorldRank()
yNrm = El.Nrm2(y)
if rank == 0:
  print "|| y ||_2 =", yNrm

x = El.LeastSquares(A,y)
xNrm = El.Nrm2(x)
El.Display( x, "x" )
if rank == 0:
  print "|| x ||_2 =", xNrm
El.SparseMultiply(El.NORMAL,-1.,A,x,1.,y)
El.Display( y, "A x - y" )
eNrm = El.Nrm2(y)
if rank == 0:
  print "|| A x - y ||_2 / || y ||_2 =", eNrm/yNrm

# Require the user to press a button before the figures are closed
commSize = El.mpi.Size( El.mpi.COMM_WORLD() )
El.Finalize()
if commSize == 1:
  raw_input('Press Enter to exit')
{% endhighlight %}
