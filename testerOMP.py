from subprocess import Popen, PIPE

n_lengths = [int(1e+6), int(1e+7), int(1e+8)]
p_counts = [2, 4, 8, 16, 32, 48, 64, 80]

iterations = 20
print(f"\nIterations per test: {iterations}")

for n in n_lengths:
  print("\nOMP, n:","{:.0e}".format(n))
  print("   processors   |  triangle probability  |   obtuse probability   |     average time(s)")
  print("-------------------------------------------------------------------------------------------")
  for pr_i in p_counts:
    avg_trig = 0
    avg_obt = 0
    avg_time = 0
    program_path = f"./BrokenTriangleOMP {pr_i} {n} RES_ONLY"
    for i in range(iterations):
      p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
      trig_prob_full = p.stdout.readline().strip().decode()
      obt_prob_full = p.stdout.readline().strip().decode()
      time_s = p.stdout.readline().strip().decode()
      avg_trig += float(trig_prob_full)
      avg_obt += float(obt_prob_full)
      avg_time += float(time_s)

    avg_trig = avg_trig / iterations
    avg_obt = avg_obt / iterations
    avg_time = avg_time / iterations
    print(f"\t{pr_i}\t| {avg_trig:.20f} | {avg_obt:.20f} | {avg_time:.20f}")
