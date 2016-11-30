/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute 	      sortering
 ----------------------------------
    150		46ms		13ms
    200		116ms		22ms
    300		322ms		55ms
    400		750ms		92ms
    800		5876ms		406ms
   1600		47280ms		1638ms
   3200		375615ms	7167ms
   6400		-		31029ms
  12800		-		134920ms


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(N⁴) Fyra nästlade for-loopar, som går igenom N punkter i värsta fall.

Sortering:O(N²log(N)) Loopar över alla punkter och för varje punkt sorterar (Nlog(N)). Jämförelse av 3 noder i taget har lägre komplexitet än sorteringen, däför påverkar ej.
