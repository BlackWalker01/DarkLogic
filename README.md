![DarkLogic Logo](https://github.com/BlackWalker01/DarkLogic/blob/master/DarkLogic/dk_logo.png)

--------------------------------------------------------------------------------

## Overview
The DarkLogic Project contains a logic based game that allows an agent (Human or AI) to 
demonstrate theorems. The game uses the DarkLogic library API as engine.

## Installation
### Requirements
The DarkLogic Project is a C++ project and requires C++20 to be built. 
With Windows, I highly recommend to use Visual Studio 2019 with Clang 10 or more.

## DarkLogic Library
### Operators
The DarkLogic Library provides an API to create and manipulate a theorem with logic rules.
The logic implemented is classical and based on 6 main operators:

| Name | Symbol | Priority | Associativity |
| ---- | --- | -- | - |
| And | && | 13 | Left |
| Equivalent | <=> | 18 | Left |
| Hypothesis | { } | 18 | Right |
| Implication | => | 19 | Left |
| Not | ! | 3 | Left |
| Or | \|\| | 14 | Left |

NB: 
About Hypothesis operator, an image worth a thousand words -> 
({p1,...,pn} r) <=> ((p1&&...&&pn) =>r)


### Inference Rules
Following table shows all inference rules in DarkLogic

| Name | Content |
| ---- | --- |
|ax | true<=>({p,HYP}p)|
|&&I| (({HYP}p)&&({HYP}q))<=>({HYP}p&&q)|
|&&E|({HYP}p&&q)<=>(({HYP}p)&&({HYP}q))"))|
|&&Eg|({HYP}p&&q)=>({HYP}p)))|
|&&Ed|({HYP}p&&q)=>({HYP}q)|
|\|\|I|(({HYP}p)\|\|({HYP}q))<=>({HYP}p\|\|q)|
|\|\|Ig|({HYP}p)=>({HYP}p\|\|q)|
|\|\|Id|({HYP}q)=>({HYP}p\|\|q)|
|\|\|E|(({HYP}p\|\|q)&&({p,HYP}r)&&({q,HYP}r))=>(({HYP}r)|
|=>I|({p,HYP}q)<=>({HYP}p=>q)"))|
|=>E|(({HYP}p)&&({HYP}p=>q))=>({HYP}q)|
|<=>I|({HYP}p=>q) && ({HYP}q=>p) <=>({HYP}p<=>q)|
|FI|({HYP}(p&&!p))<=>({HYP}false)|
|FE|({p,HYP}false)<=>({HYP}!p)|
|FI!|({!p,HYP}p)<=>({!p,HYP}false)|
|weakening|({HYP}p)=>({q,HYP}p)|
|arr|({!p,HYP}false)<=>({HYP}p)|
|!!|({HYP}!!p)<=>({HYP}p)|

## Greetings
Special Thanks to RedWalker for the DarkLogic Logo

## License
DarkLogic is a project under CC0 license. 
See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.