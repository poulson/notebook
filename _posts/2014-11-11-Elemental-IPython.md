---
layout: post
title: Experimenting with Elemental+IPython
date: 2014-11-11 17:02:00
location: Stanford (Stanford, CA)
author:
    twitter: libelemental
categories: notes
---

<center><img src="{{ site.baseurl }}/assets/ElementalPlusIPython.png"></center>

\\

While it is trivial to run an MPI-based Python program interactively with
a single process, there is no straightforward means of running an interactive
distributed Python session with a nontrivial number of processes using the 
vanilla python interpreter. Thankfully, [IPython](http://ipython.org/) provides
a convenient [hub/engine/client abstraction](http://ipython.org/ipython-doc/stable/parallel/parallel_intro.html) which solves the problem. In the end, a 
[surprisingly small amount of modifications to Elemental](https://github.com/elemental/Elemental/commit/5711a7a371d824f785f23a36c95e2bfd495996b7) were 
required in order to iron out its usage from IPython. 

### An introduction to interactive MPI via IPython

At a high level, the main idea is for an IPython *controller* process to 
handle launching a python interpreter (or, *engine*) for each MPI process, and 
for a *client* IPython instance to handle interacting with the user and to 
transfer said commands to the *controller*, which then broadcasts the commands
to the *engines*.

On a vanilla Linux distribution, [adding an MPI profile for IPython is 
often relatively straightforward](http://nbviewer.ipython.org/github/ipython/talks/blob/master/parallel/Part%207%20-%20Working%20with%20MPI.ipynb) and can 
typically be handled by first running the command

{% highlight sh %}
ipython profile create --parallel --profile=mpi
{% endhighlight %}

followed by adding the line

{% highlight python %}
c.IPClusterEngines.engine_launcher_class = 'MPI'
{% endhighlight %}

to the file ``~/.ipython/profile_mpi/ipcluster_config.py``.

Once the profile is set up, a controller with six engines should be launched
via the command:

{% highlight sh %}
ipcluster start -n 6 --profile=mpi
{% endhighlight %}

An ``ipython`` instance should then be started, and the connection can be 
tested via the commands:

{% highlight python %}
from IPython.parallel import Client
c = Client(profile='mpi')
c.ids
{% endhighlight %}

which should output ``[0, 1, 2, 3, 4, 5]``. In order for subsequent interactive
Python routines to be executed using all of the (six) available processes, 
the input cell should begin with the special cell marker ``%%px``, and, if their
output is to be displayed in the notebook, the command ``c[:].block = True`` 
should be run so that the parallel cells are run synchonously. Furthermore,
it is usually preferred for Matplotlib figures to be displayed within the 
notebook, in which case the special command ``%matplotlib inline`` should be
run near the beginning of the script.

### An example Elemental notebook

{% include notebooks/FoxLi.html %}
