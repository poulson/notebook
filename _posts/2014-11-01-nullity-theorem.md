---
layout: post
title: A simplified proof of the nullity theorem
date: 2014-11-01 21:52:00
edit: 2014-11-03 14:54:00
location: KAUST (Thuwal, Saudi Arabia)
author:
    twitter: libelemental
categories: notes
---

The [nullity theorem](http://en.wikipedia.org/wiki/Nullity_theorem), which was
originally proven by Gustavson (1984) and Fiedler and Markham (1986), is a 
surprising statement about equivalences between the dimensions of the nullspace
(aka, the *nullity*) of different subblocks of a matrix and its inverse which 
allows for powerful statements about the ranks of subblocks of inverses of 
structured matrices
(e.g., the rank of the top-right quadrant of the inverse of a tridiagonal 
matrix is at most one). In particular, the partitions must be chosen so that 
blockwise multiplication of the matrix and its inverse makes sense.

## Nullity theorem
Let $$ A \in \mathbb{C}^{N \times N} $$ have an inverse 
$$ B \equiv A^{-1} $$. Then 

$$ \text{nullity}(A(\mathcal{I},\mathcal{J})) = 
   \text{nullity}(B(\mathcal{J}^C,\mathcal{I}^C)) $$,

where $$ \mathcal{I} $$ and $$ \mathcal{J} $$ are arbitrary index sets, and 
$$ \mathcal{I}^C $$ and $$ \mathcal{J}^C $$ represent their complements with
respect to the set $$ [0,N) \subset \mathbb{N} $$.

### Proof mechanism

While a complete proof is given in Fiedler and Markham's paper, with the 
following lemma being the crucial argument, the final result can be reached 
almost immediately from the lemma using simple applications of permutation
matrices. Indeed, this is the main reason behind the existence of this note.

## Lemma
Let $$ A \in \mathbb{C}^{N \times N} $$ have an inverse 
$$ B \equiv A^{-1} $$ and partition the two matrices as

$$ A = \begin{pmatrix} A_{1,1} & A_{1,2} \\ A_{2,1} & A_{2,2} \end{pmatrix},
   \;\;\;
   B = \begin{pmatrix} B_{1,1} & B_{1,2} \\ B_{2,1} & B_{2,2} \end{pmatrix}, $$ 

where $$ A_{1,1} \in \mathbb{C}^{m \times n} $$ and 
$$ B_{1,1} \in \mathbb{C}^{n \times m} $$. Then 
$$ \text{nullity}(A_{1,1}) \ge \text{nullity}(B_{2,2}) $$.

### Proof of Lemma
Since

$$ \begin{pmatrix} A_{1,1} & A_{1,2} \\ A_{2,1} & A_{2,2} \end{pmatrix}
   \begin{pmatrix} B_{1,1} & B_{1,2} \\ B_{2,1} & B_{2,2} \end{pmatrix} =
   \begin{pmatrix} I & 0 \\ 0 & I \end{pmatrix}, $$

we have that 

$$ A_{1,1} B_{1,2} + A_{1,2} B_{2,2} = 0. $$

Let $$ k $$ be the dimension of the nullspace of $$ B_{2,2} $$ and the columns
of $$ F $$ form a basis for this nullspace.
Then $$ A_{1,1} (B_{1,2} F) = 0 $$, and since $$ B_{1,2} F $$ must continue 
to have linearly independent column (since otherwise $$ [B_{1,2};B_{2,2}] $$ would not be of full-rank, implying $$ B $$ is not invertible), 
$$ \text{nullity}(A_{1,1}) \ge \text{nullity}(B_{2,2}) $$. $$ \blacksquare $$

## Corollary 1
$$ \text{nullity}(A_{1,1}) = \text{nullity}(B_{2,2}) $$

### Proof
Apply the lemma to $$ \hat A $$ and $$ \hat B $$, where 
$$ \hat A \equiv P B Q^H $$, where $$ P $$ and $$ Q $$ are permutation matrices
chosen such that

$$ P B Q^H = 
  \begin{pmatrix} B_{2,2} & B_{2,1} \\ B_{1,2} & B_{1,1} \end{pmatrix} $$,

which implies that 
$$ \text{nullity}(\hat A_{1,1}) \ge \text{nullity}(\hat B_{2,2}) $$. 
$$ \blacksquare $$

## Corollary 2
$$ \text{nullity}(A_{1,2}) = \text{nullity}(B_{1,2}) $$

### Proof
Apply Corollary 1 to the matrices $$ \hat A \equiv A P^H $$ and $$ \hat B \equiv (\hat A)^{-1} $$, where $$ P $$ is a permutation matrix chosen such 
that

$$ A P^H = 
  \begin{pmatrix} A_{1,2} & A_{1,1} \\ A_{2,2} & A_{2,1} \end{pmatrix}, $$

which also implies that

$$ \hat B = P B = 
   \begin{pmatrix} B_{2,1} & B_{2,2} \\ B_{1,1} & B_{1,2} \end{pmatrix} $$,

and so Corollary 1 immediately implies the result. $$ \blacksquare $$

## Corollary 3
$$ \text{nullity}(A(\mathcal{I},\mathcal{J})) = \text{nullity}(B(\mathcal{J}^C,\mathcal{I}^C)) $$.

### Proof
Simply choose permutation matrices $$ P $$ and $$ Q $$ such that $$ P A Q^H $$ 
places $$ A(\mathcal{I},\mathcal{J}) $$ into its upper-left corner and apply 
Corollary 1. $$ \blacksquare $$

### Acknowledgements
This theorem was originally mentioned to me in the context of the ranks of the
off-diagonal blocks of weakly-admissible $$ \mathcal{H} $$-matrices by [Leonardo
Zepeda](http://math.mit.edu/icg/people/profiles.html#Leonardo) while visiting
[Laurent Demanet's Imaging and Computing Group](http://math.mit.edu/icg/).
The simplified proof was worked out in a discussion with 
[Yingzhou (Ryan) Li](http://web.stanford.edu/~ryanlee/) and this note was 
encouraged by a discussion during a visit to 
[David Keyes' Extreme Computing Research Center](http://ecrc.kaust.edu.sa/Pages/Home.aspx).
