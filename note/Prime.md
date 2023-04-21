* Prime
  - Define  
    $$\nexists a \in (\mathbb Z^+ - \{1, p\}), \text{ let } a | p  \tag{Prime}$$ 
    A number that can only be divided by $1$ and itself.

  - Property
    * Fundamental Theorem of Arithmetic  
      Any integer $n$ greater than $1$ can be uniquely expressed in the form of prime $p_i$ product.   
      $$n = \prod_i p_i^{\alpha_i} \quad n \in \mathbb Z, n > 1$$

    * Fermat's Little Theorem  
      $$a^{p-1} \equiv 1 \mod p$$
      Where $p$ is a prime and $a$ is an any integer that is not a multiple of $p$.

    - Problem: Resolving prime factor  
      - Pollard Rho algorithm

    - Problem: Filter Prime Number from a range of numbers
      * Euler's Sieve
        - Purpose  
          For a numbers range from $2$ to $n$, we aim to sift out all primes from them, through let each composite number be screened by its minimum prime factor.

        - Process   
          First, We traverse the numbers $x$ range from $2$ to $n$, we join $x$ into prime set $S_p$ if $x$ is not marked as non-prime.

          Meanwhile, we traversing the current prime table $p \in S_p$, and mark $p x$ as non-prime. When $p | x$, we should stop traversing the prime table, because that the primes $p' \in S_p$ large than $p$ are no longer the minimum prime factor of $p' x$ ($p' x = p' p r$).

          ```py

          def EulerSieve(n):
            primes = []
            is_p = ones(n + 1)

            for i in range(2, n + 1):
                if (is_p[i]):
                    primes.append(i)

                for p in primes:
                    if (p * i > n):
                        break

                    is_p[p * i] = 0

                    if (i % p == 0):
                        break
            return primes
          ```  

  - Include
    * Mersenne Prime