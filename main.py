import math
import random

PRINT_ = False
pi_half = math.pi / 2
# unoptimized condition, acos is very expensive to compute
def get_angs(s1, s2, s3):
	return math.acos((s2**2 + s3**2 - s1**2) / (2 * s2 * s3))

# check conditions for being obtuse
def obtuse_condition(s1, s2, s3):
	c1 = get_angs(s1, s2, s3) > pi_half
	c2 = get_angs(s2, s1, s3) > pi_half
	c3 = get_angs(s3, s1, s2) > pi_half
	return c1 or c2 or c3

# optimized condition, its obtuse if len is equal or lower than zero
def get_len(s1, s2, s3): 
	return (s2**2 + s3**2 - s1**2) 
  # / (2 * s2 * s3))

# check conditions for being obtuse
def obtuse_condition_optimized(s1, s2, s3):
	c1 = get_len(s1, s2, s3) < 0 
	c2 = get_len(s2, s1, s3) < 0 
	c3 = get_len(s3, s1, s2) < 0 
	return c1 or c2 or c3

# check conditions for being a triangle
def side_condition(s1, s2, s3):
	c1 = s1 <= (s2 + s3)
	c2 = s2 <= (s1 + s3)
	c3 = s3 <= (s1 + s2)
	c4 = s1 != 0 and s2 != 0 and s3 != 0
	return c1 and c2 and c3 and c4

# check both conditions and return a boolean for each
def check_conditions(a_, b_, c_):
	isTriangle = False
	isObtuse = False

	if side_condition(a_, b_, c_):
		if PRINT_: print('is triangle')
		isTriangle = True
		# if obtuse_condition(a_, b_, c_):
		if obtuse_condition_optimized(a_, b_, c_):
			if PRINT_: print('is obtuse')
			isObtuse = True
		else:
			if PRINT_: print('is not obtuse')
	else:
		if PRINT_: print('is not triangle')

	return isTriangle, isObtuse

# run one instance of experiment with set lenght and precision
def experiment(interval_, length_, precision_):

  # random points in interval
	p1 = random.randint(0, interval_)
	p2 = random.randint(0, interval_)

  # set p1 as min and p2 as max
	if p1 > p2:
		temp = p1
		p1 = p2
		p2 = temp

  # create sub intervals
	i_1 = p1 - 0
	i_2 = p2 - p1
	i_3 = interval_ - p2

	if PRINT_:
		print("interval:")
		print(f"{0:.{precision_}f}" + " to " + f"{length_:.{precision_}f}\n")
		print("segments:")
		print(f"{i_1/(10**precision_):.{precision_}f}")
		print(f"{i_2/(10**precision_):.{precision_}f}")
		print(f"{i_3/(10**precision_):.{precision_}f}\n")

  # check triangle and obtuse conditions for given sub intervals
	trig, obt = check_conditions(i_1, i_2, i_3)

	return trig, obt

# Simultation
def monte_carlo_sim(iterations=10000, length=1, precision=1, experiments=10):
  for t in range(experiments):

    # Monte Carlo Simulation (run expermient
    # n times and get average of results to
    # obtain approximate probability)
    trig_hist = []
    obt_hist = []
    interval = length * (10**precision)
    for i in range(iterations):
      trig, obt = experiment(interval, length, precision)
      trig_hist.append(trig)
      obt_hist.append(obt)

    trig_prob = sum(trig_hist) / iterations
    obt_prob = sum(obt_hist) / iterations
    obt_abs_prob = sum(obt_hist) / sum(trig_hist)

    

    print("experiment", t, ":", trig_prob, obt_prob)
    # print("absolute obtuse probability:", obt_abs_prob)



monte_carlo_sim(precision=6, iterations = 100000)
