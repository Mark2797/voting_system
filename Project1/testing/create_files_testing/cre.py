# THIS EXISTS AS A FRAME WORK FOR create_test_files.cpp AND SHOULD NOT BE USED.
# import numpy as np
# import sys 
# import time as t


# np.set_printoptions(threshold=sys.maxsize)
# names1 = ["Chuck Lancaster", "Mark Suckerberg", "Andrew Hero", "Micheal Ashton", "Joe Cool", "Jimmy Donaldson", "John Kennedy", "Patrick Star", "Robot Iam", "Alice Wonder"]
# names2 = names1[:5]
# names_list = [names2] * 4 + [names1] * 4
# ballots = [100] * 4 + [100000] * 4
# type = ["S", "P"] * 4
# path = [f'test_file{n}.csv' for n in range(1, len(names_list) + 1)]

# t0 = t.time()
# for f, ballot, names, election in zip(path, ballots, names_list, type):
#     with open(f, 'w') as f:
#         length = list(range(1, len(names) + 1))
#         string = ','.join(names)
#         f.write(f'{string}\n')
#         if election == "S":
#             vals = np.array([np.random.permutation(length) for _ in range(ballot)])
#             np.savetxt(f, vals, delimiter=",", fmt="%d")
#         elif election == "P":
#             vals = np.zeros((ballot, len(length)), dtype=int)
#             random_indices = [np.random.choice(len(length)) for _ in range(ballot)]
#             vals[np.arange(ballot), random_indices] = 1
#             np.savetxt(f, vals, delimiter=",", fmt="%d")
# t1 = t.time()
# diff = t1-t0
# print(f'{diff:.2f} seconds to generate {len(path)} test files.')