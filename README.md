# Reference

[Transaction Fraud Detection Based on Total Order Relation and Behavior Diversity](./Transaction_Fraud_Detection_Based_on_Total_Order_Relation_and_Behavior_Diversity.pdf)

# Definition

* **Record**
  * Meaning: A transaction record
  * <font color=blue>*Symbol:*</font> $r=\{a_1,a_2,...,a_m\}$​
    * $a_i$​ is attribute of the transactions
    * $m$ different attributes
      * For example $a_i$ may be:
        * Transaction date
        * Transaction amount
        * etc.
* **Attribute Sets**
  * Meaning: A set with one attribute's data of all possible values
  * <font color=blue>*Symbol:*</font> 
    * $A_i=\{a_1^i,a_2^i,...,a_{n_i}^i\}$​​​​
      * $a_{n^i}^i$ is the possible attribute's value
      * For a record, $r=\{a_1,a_2,...,a_m |a_i \in A_i,i\in (1,m),i\in N^+\}$ 
    * $n_i=|A_i|$​
      * <font color=green>The size of attribute sets</font>

* **User Log**

  * Meaning: A user that can make different transactions. All these transactions for one user become the user log
  * <font color=blue>*Symbol:*</font>
    * $L_u=\{r_1^u,r_2^u,...,r_{n^u}^u\}$​​
      * $r_{i^u}^u$ is the record of user $u$​
      * $r_{i^u}^u$ *are possible to be same and will be all kept*
    * $n^u=|L_u|$
      * <font color=green>How many transaction record of one user have</font>

* **All different records in an user log**

  * Meaning: Since the record in the user log may be same, then we use this set to represent all unique records in an user log
  * <font color=blue>*Symbol:*</font> $R_u$​

  <a id=LGBP></a>

* <font color=dark red>**Logic graph of BP (LGBP)**</font>

  * <font color=blue>*Symbol:*</font>

    * $$
      \begin{align}
      A_1^u &= \{a\in A_1|\exist r\in R_u:a\in r\} \\
      A_2^u &= \{a\in A_2|\exist r\in R_u:a\in r\} \\
      ... \\
      A_m^u &= \{a\in A_m|\exist r\in R_u:a\in r\} \\
      \end{align}
      $$

      * $A_i^u \subseteq A_i$

    * $n_i^u=|A_i^u|$ for $i\in\{1,2,...,m\}$​ 

    * <font color=blue>*The LGBP of $u$ is a directed acyclic graph*</font>

      * $G_u=(V_u,E_u)$
        * $V_u=\{a_s,a_e\}\cup A_1^u\cup A_2^u\cup...\cup A_m^u$
          * $a_s$ and $a_e$ are two *special nodes that represent the start and end of a transaction*
        * $\forall a\in A_1^u, (v_s,a)\in E_u$
        * $\forall a\in A_m^u, (a,v_e)\in E_u$​
        * $\forall i\in \{1,2,...,m-1\}$, $\forall a\in A_i^u$, $\forall a'\in A_{i+1}^u:(a,a')\in E_u$ iff $\exists r\in R_u:a\in r \land a'\in r$​

    * **Graph Example**

      * ![截屏2024-02-15 上午11.53.26](截屏2024-02-15 上午11.53.26.png)

* **Prepaths**

  * Let $G_u=(V_u,E_u)$ be the LGBP of $u$. $\forall v \in V_u$
  * Meaning: the set of all directed paths from node $a_s$ to node $v$ in $G_u$

* **Postnodes**

  * Let $G_u=(V_u,E_u)$ be the LGBP of $u$. $\forall v \in V_u$
  * Meaning: the set of *nodes* that are directly reached from $v$ in $G_u$​

  <a id=OM></a>

* <font color=dark red>**Path-Based Probability Transition Matrix (OM)**</font>

  * Let $G_u=(V_u,E_u)$ be the LGBP of user $u$. $\forall v\in V_u$,  $M_v$ is a $|\text{prepaths(}v\text{)})| \times |\text{postnodes(}v\text{)}|$ matrix where $\forall \sigma \in \text{prepaths(}v\text{)}),\forall v' \in \text{postnodes(}v\text{)}$​​

  * Meaning: the transition probability from $v$ to $v'$ under the condition that $v$ is reached via $\sigma$

  * <font color=blue>*Symbol*</font>

    * $M_v(\sigma, v')=P(v \rightarrow v'|\sigma)$​

  * **Example**

    * Node $(0,200]$ has *five directed paths* from $a_s:\sigma_1 - \sigma_5$, and has *two postnodes*: $SJ$ and $AX$. Then we have a $5 \times 2$ matrix:

      * $$
        M_{(0,200]} = 
        
        \begin{bmatrix}
        P((0,200] \rightarrow SJ|\sigma_1) & P((0,200] \rightarrow AX|\sigma_1) \\
        P((0,200] \rightarrow SJ|\sigma_2) & P((0,200] \rightarrow AX|\sigma_2) \\
        P((0,200] \rightarrow SJ|\sigma_3) & P((0,200] \rightarrow AX|\sigma_3) \\
        P((0,200] \rightarrow SJ|\sigma_4) & P((0,200] \rightarrow AX|\sigma_4) \\
        P((0,200] \rightarrow SJ|\sigma_5) & P((0,200] \rightarrow AX|\sigma_5) \\
        \end{bmatrix}
        $$

      * $$
        M(0,200) = 
        \begin{array}{c|cc}
            & SJ & AX \\
        \hline
        \sigma_1 & 1 - \omega_u & 0 \\
        \sigma_2 & 0 & 0 \\
        \sigma_3 & 0 & 1 - \omega_u \\
        \sigma_4 & 0 & 0 \\
        \sigma_5 & \frac{1 - \omega_u}{2} & \frac{1 - \omega_u}{2} \\
        \end{array}.
        $$

  <a id=DC></a>

* **Diversity Coefficient **

  * Meaning: used to calculate transaction probability
    * *The greater the value of $\omega_u$, the higher the probability of conducting a transaction which never took place in the historical records of the user*
  * <font color=blue>*Symbol*</font>
    * $\omega_u=-\sum_{i\in R_u}P(r) \times log_{\kappa}P(r)$
      * $P(r)$ is the probability of record $r$ occurring in $L_u$​
        * <font color=green>Calculated by the frequency of $r$ in $L_u$</font>
      * $\kappa = \text{max}_{u\in U}\{|R_u|\}$
        * $U$ is the set of all users

* **Frequency of Directed Path $\sigma$**

  * $f(\sigma)=|\{r\in L_u | \forall v \in \#(\sigma)/\{a_s\}:v\in r$}|
    * $\#(\sigma)$ is the set of all nodes in $\sigma$

* **Transition Probability**

  * Meaning: the transition probability from $v$ to $v'$ under the condition that $v$ is reached from $a_s$ via $\sigma$:

    * $$
      P(v \to v'|\sigma) = 
      \begin{cases} 
      (1 - \omega_{v}) \times \frac{f(\sigma^{v'})}{f(\sigma)} & \text{if } f(\sigma) \neq 0 \\
      0 & \text{if } f(\sigma) = 0.
      \end{cases}
      $$

  <a id=STPM></a>

* **State Transition Probability Matrix**

  * $\Tau=[\tau_{ij}]$

    * $$
      \begin{align}
      \tau_{ij}
      &= P(q_{t+1=C_j}|q_t=C_i),1\le i,j\le N \\
      &= \frac{f(C_i \rightarrow C_j)}{f(C_i)} \\
      &= \frac{|\{\{r,r'\} \subseteq L_u|C_i \in r \land C_j \in r' \}|}{|\{r\in L_u / \{r_{n^u}^u\}|C_i\in r\}|}
      \end{align}
      $$

      * $f(C_i)=|\{r\in L_u / \{r_{n^u}^u\}|C_i\in r\}|$​
      * $f(C_i \rightarrow C_j)=|\{\{r,r'\} \subseteq L_u|C_i \in r \land C_j \in r' \}|$

  * $\mathcal{C}=\{C_1,C_2,...,C_N\}$​ 

    * The set of *categories of goods*

  * *The good category in the transaction at time $t$* is denoted by $q_t$​

  * ![截屏2024-02-15 下午4.29.15](./截屏2024-02-15 下午4.29.15.png)

* <font color=dark red>***Behavior Profile***</font>

  * Let $L_u=\{r_1^u,r_2^u,...,r_{n^u}^u\}$ be the transaction log of user $u$
  * <font color=blue>Symbol</font>
    * $BP_u=(V_u,E_u,\mathcal{M}_u,\Tau_u,\omega_u)$​​
      * $G_u=(V_u,E_u)$ is the [LGBP](#LGBP) of $u$
      * $\mathcal{M}_u=\{M_v|v\in V_u\}$ is the set of [path-based transition probability matrices](#OM) of all nodes in $G_u$
      * $\Tau_u$ is the [state transition probability matrix](STPM) w.r.t. $u$
      * $\omega_u$ is the [diversity coefficient](#DC) of $u$

  # Data Example

  <img src="截屏2024-02-13 上午11.52.39.png" alt="截屏2024-02-13 上午11.52.39" style="zoom:200%;" />

  * This table list one example of user log which contain $6$ transaction records from this user
  * <font color=dark red>**The data need to be preprocessed into categories**</font>
  * Even the same $r$ will be saved in the log
  * **All this attributes can be ordered**
    * For example, *transaction_location* can be previous to *category_of_good*. Since user need to login before buy anything
      * Therefore, we can arrange them in order:
      * $A_1 \prec A_2 \prec ... \prec A_m$