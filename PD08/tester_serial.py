from subprocess import Popen, PIPE

iterations = 100

print("   processors", "  |\t","average(s)")
print("-------------------------------------")

avg = 0
program_path = f"./serial -noevalprints"
for i in range(iterations):
  p = Popen([program_path], shell=True, stdout=PIPE, stdin=PIPE)
  p.stdin.flush()
  result = p.stdout.readline().strip().decode()
  avg += float(result)
avg = avg / iterations
print("\t",1, "\t|  ",f'{avg:.20f}')
