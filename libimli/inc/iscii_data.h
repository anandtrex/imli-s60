/* 
 * Automatically generated using iscii_bin2h.pl
 * on 4-1-2007, 21:33
 * Additional changes for 4th level conjuncts and cleanup of the generic.*
 * on 21-April-2007
 */

/* 
 * contents of generic.spl
 bindu
 visarg           
 cbindu          
 viram            
 avagrah          
 excla            
 quote            
 lpar             
 rpar             
 comma            
 hyphen           
 dot              
 slash            
 semcol           
 qmark            
 short-a     
*/

const static byte_t imli_to_iscii_spl[] = {
    0xA2, 0xA3, 0xA1, 0xEA, '@', '!', '\'', '(', ')', ',', '-', '.', '/', ';', '?', 0xAE
};

/*
 contents of generic.vow

*/
const static byte_t imli_to_iscii_vow[] = {
    0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xE8
};

const static byte_t imli_to_iscii_vow_matras[] = {
    '@', 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8 
};

/* 
   contents of generic.con 

   ka      1       
   kha     2       
   ga      3       
   gha     4       
   nga     5       
   cha     6       
   chha    7       
   ja      8       
   jha     9       
   nja     10      
   ta      11      
   tta     12      
   dda     13      
   ddha    14      
   nna     15      
   tha     16      
   ttha    17      
   da      18      
   dha     19      
   na      20      
   nex     21      #additional space for na#
   nas     22      #tamil na moved up#
   pa      23      
   pha     24      
   ba      25      
   bha     26      
   ma      27      
   ya      28      
   yab     29      #ya for bangla, oriya and asamiya, no changes#
   ra      30          
   rex     31	   #first additional space for ra#
   rxl     32 	   #second additional space for ra#	
   rxxl    33	   #third additional space for ra#
   rra     34	   # ra marathi, kannada, moved up#
   la      35       
   lla	   36	   #hard la moved up# 
   zha 	   37	   #retroflex la tamil and malayalam#
   va      38       
   ca      39      
   sha     40      
   sa      41      
   ha      42       
   ddra	   43      #moved from earlier position 16# 
   ddrha   44      #moved from earlier position 18#	
   ksha	   45	   #moved from earlier position 2#
   jnya	   46	   #moved from earlier position 10#
   qa	   47	   #added for ka plus nukhta#
   xa	   48	   #added for kha plus nukhta#
   gga	   49	   #added for ga plus nukhta#
   za	   50	   #added for ja plus nukhta#
   fa	   51	   #added for pha plus nukhta#
   cca	   52	   #added for ca plus nukhta oriya#
   vedic   53
   nukhta  54

*/

/* NOTE: consonants start from 1*/
const static byte_t imli_to_iscii_con[] = {
    '?', 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA,
    0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3,
    0xC4, 0xC5, 0xC6, 0x20, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB,
    0xCC, 0xCD, 0xCE, 0xCF, 0x20, 0x20, 0x20, 0xD0, 0xD1,
    0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0xE9
};

