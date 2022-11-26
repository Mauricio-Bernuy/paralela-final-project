``` c
/* SHARED MEMORY */

global trig_prob, obt_prob = 0

forall i ∈ {1,..n} pardo:
  // random points in interval
	p1 = RandomUniform{0:1}
  p2 = RandomUniform{0:1}
  p1, p2 = swapMinMax(p1, p2)

  // generate sub intervals
  i1, i2, i3 = toSubintervals(p1,p2)
  
  is_trig, is_obt = false

  // triange conditions
  is_trig = i1 <= (i2 + i3) ∧
            i2 <= (i1 + i3) ∧ 
            i3 <= (i1 + i2) ∧
            s1 != 0 ∧ s2 != 0 ∧ s3 != 0
            
  // obtuse conditions
  if is_trig == true:
    is_obt = ACOS((s2**2 + s3**2 - s1**2) / (2 * s2 * s3)) > PI / 2 ∨ 
             ACOS((s1**2 + s3**2 - s2**2) / (2 * s1 * s3)) > PI / 2 ∨ 
             ACOS((s1**2 + s2**2 - s3**2) / (2 * s1 * s2)) > PI / 2 

  // add to global values
  trig_prob += is_trig
  obt_prob += is_obt

// mean
trig_prob /= n
obt_prob /= n

return trig_prob, obt_prob

  
/* DISTRIBUTED MEMORY */

global trig_prob, obt_prob = 0
local sub_trig_prob, sub_obt_prob = 0

forall i ∈ {1,..n} pardo:
  // random points in interval
	p1 = RandomUniform{0:1}
  p2 = RandomUniform{0:1}
  p1, p2 = swapMinMax(p1, p2)

  // generate sub intervals
  i1, i2, i3 = toSubintervals(p1,p2)
  
  is_trig, is_obt = false

  // triange conditions
  is_trig = i1 <= (i2 + i3) ∧
            i2 <= (i1 + i3) ∧ 
            i3 <= (i1 + i2) ∧
            s1 != 0 ∧ s2 != 0 ∧ s3 != 0
            
  // obtuse conditions
  if is_trig == true:
    is_obt = ACOS((s2**2 + s3**2 - s1**2) / (2 * s2 * s3)) > PI / 2 ∨ 
             ACOS((s1**2 + s3**2 - s2**2) / (2 * s1 * s3)) > PI / 2 ∨ 
             ACOS((s1**2 + s2**2 - s3**2) / (2 * s1 * s2)) > PI / 2 
  
  // add to local values
  sub_trig_prob += is_trig
  sub_obt_prob += is_obt

// reductions
reduce({sum each sub_trig_prob into trig_prob}) // log(N)
reduce({sum each sub_obt_prob into obt_prob}) // log(N)

// mean
trig_prob /= n
obt_prob /= n

return trig_prob, obt_prob


```
