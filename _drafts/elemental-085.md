---
layout: post
title: Elemental's status as of version 0.85
date: 2014-11-10 07:42:00
location: Stanford (Stanford, CA)
categories: notes
---

<img src="{{ site.baseurl }}/assets/elemental.png">

More than five months had passed since the last release of Elemental, and so 
[version 0.85 of the library](http://libelemental.org/releases/0.85/) was long 
overdue. In addition to a number of bug fixes and more consistent naming 
conventions, a number of significant features were added, such as sparse-direct 
functionality, generalized interfaces to distributed-memory functionality via 
read/write proxies, and nearly-complete C and python interfaces.

### What\'s new

#### Sparse-direct functionality
**TODO**

#### Read/write proxies
**TODO**

#### C interface
**TODO: Intro**

{% highlight C %}
#include "El.h"

int main( int argc, char* argv[] )
{
    ElError error = ElInitialize( &argc, &argv );
    if( error != EL_SUCCESS )
        MPI_Abort( MPI_COMM_WORLD, 1 );

    ElInt m, n;
    error = ElInput_I("--m","matrix height",100,&m);
    EL_ABORT_ON_ERROR( error );
    error = ElInput_I("--n","matrix width",100,&n);
    EL_ABORT_ON_ERROR( error );

    ElGrid grid;
    error = ElDefaultGrid( &grid );
    EL_ABORT_ON_ERROR( error );

    ElDistMatrix_d A;
    error = ElDistMatrixCreate_d( grid, &A );
    EL_ABORT_ON_ERROR( error );
    error = ElDistMatrixResize_d( A, m, n );
    EL_ABORT_ON_ERROR( error );
    ElInt i, j;
    for( j=0; j<n; ++j )
    {
        for( i=0; i<m; ++i )
        {
            error = ElDistMatrixSet_d( A, i, j, (double)i-j );
            EL_ABORT_ON_ERROR( error );
        }
    }
    
    error = ElDisplayDist_d( A, "A(i,j) = i-j" );
    EL_ABORT_ON_ERROR( error );

    error = ElFinalize();
    if( error != EL_SUCCESS )
        MPI_Abort( MPI_COMM_WORLD, 1 );
    return 0;
}
{% endhighlight %}

#### ![Python]({{ site.baseurl }}/assets/python-small.png) interface
**TODO: Intro**

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

# Set up the linear operator as the vertical concatenation of a negative
# 2D Laplacian and an identity matrix.
A = ExtendedLaplacian(n0,n1)
El.Display( A, "A" )
El.Display( A.DistGraph(), "Graph of A" )

# Form the right-hand side as a uniformly-sampled distributed vector
y = El.DistMultiVec()
El.Uniform( y, 2*n0*n1, 1 )
El.Display( y, "y" )

# Solve the least-squares problem and display the relative error norm
yNrm = El.Nrm2(y)
x = El.LeastSquares(A,y)
El.SparseMultiply(El.NORMAL,-1,A,x,1,y)
El.Display(y,"A x - y")
eNrm = El.Nrm2(y)
if El.mpi.WorldRank() == 0:
  print "|| A x - y ||_2 / || y ||_2 = ", eNrm/yNrm
{% endhighlight %}

### Where to go from here?

#### Sparse Alternating Direction Method of Multipliers
**TODO**

#### Interior Point Methods
**TODO**

#### 2D (and beyond) sparse matrix application

For sparse matrices whose graphs have highly variable edge degrees, it is 
well-known that the 1D sparse matrix distributions traditionally used by 
distributed-memory PDE solvers can easily be limited to $$ O(1) $$ speedup
despite the number of available processes. Thus, [Buluç et al.](http://hpc.sagepub.com/content/25/4/496.abstract) took a queue 
from distributed-memory dense linear algebra and proposed the usage of 
2D sparse matrix distributions as part of the [Combinatorial BLAS](http://gauss.cs.ucsb.edu/~aydin/CombBLAS/html/index.html), which is the backend for their 
[Knowledge Discovery Toolbox](http://kdt.sourceforge.net/wiki/index.php/Main_Page), which, in the worst case, guarantee roughly an $$ O(\sqrt{p}) $$ parallel
speedup when $$ p $$ processes are available.

It is planned for Elemental to transition from 1D to 2D sparse matrix 
distributions in the near future and, if possible, to investigate alternative
data distributions which might guarantee closer to $$ O(p) $$ worst-case
parallel speedups (e.g., through decomposing the original sparse matrix as 
sums and/or products of a polylogarithmic number of easily-applied matrices).

#### ![Julia]({{ site.baseurl }}/assets/julia-small.png) integration

Given that Elemental's Python interface is built on top of Elemental's C 
interface using [ctypes](https://docs.python.org/2/library/ctypes.html), an
analogous strategy should be the default approach to exposing a 
[Julia](http://julialang.org/) interface.

Another goal is to experiment with the performance and productivity tradeoffs 
that would result from natively implementing/extending Elemental's DistMatrix
class within Julia.

#### [IPython](http://ipython.org/)/[IJulia](https://github.com/JuliaLang/IJulia.jl)/[Jupyter](http://jupyter.org/) integration

Due to the large number of additional conveniences and the increased pedagogical
value of [IPython notebooks](http://ipython.org/) over a vanilla Python
shell (in particular, the improved support for 
[parallel computing](http://ipython.org/ipython-doc/stable/parallel/parallel_intro.html)), it would be worthwhile to perform any necessary steps for 
transitioning Elemental's current Python interface and examples over to 
IPython. Furthermore, as a Julia interface comes online, support for 
[IJulia](https://github.com/JuliaLang/IJulia.jl) (or, as it is soon to be 
renamed, [Jupyter](https://github.com/JuliaLang/IJulia.jl)) should also be 
added.

#### [Intel MKL](https://software.intel.com/en-us/intel-mkl) integration

There is currently discussion of the possibility of incorporating Elemental 
into [Intel's MKL](https://software.intel.com/en-us/intel-mkl). Though there is 
not yet a concrete plan, making Elemental 
available by default on the vast majority of high-performance would avoid
requiring most users from spending a significant amount of time ensuring that
the library is properly configured and therefore also accelerate adoption.
I will be giving a talk at one of Intel's Sunnyvale offices over the next few
weeks and will hopefully be able to say more soon.

#### Dedicated development machine

Elemental has been primarily developed on laptops and is only periodically 
performance-tested on large clusters. There is currently an effort to procure
a development machine in order to provide an easier on-ramp for interested
developers. If a large enough machine is acquired, Elemental will be regularly
publicly benchmarked on it so that users can have a rough idea how well the 
library should perform on their own machines.

#### Making naming conventions consistent with scientific computing ecosystem

Though Elemental has historically made use of upper camel case for classes and 
functions, (lower) camel case for variable names, and 
UPPER_CASE_WITH_UNDERSCORES for constants and macros, these conventions are at
odds with the C++, python, and Julia communities at large. If members of 
Elemental's community continue to vocally oppose the current style, the issue
may be revisited in the future.

*Availability*: The source code for Elemental can be downloaded from either
the project website, [libelemental.org](http://libelemental.org) or from
[GitHub](https://github.com/elemental/Elemental.git) and is made available
under the [New BSD License](http://opensource.org/licenses/BSD-2-Clause).
