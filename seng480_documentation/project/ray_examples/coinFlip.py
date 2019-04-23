import random
import ray
import sys
import time

def flop(numTimes):
    results = 0
    for i in range(0,numTimes):
        rdm = random.randint(0,1)
        if rdm == 1:
            results += 1
    return results/numTimes

# @ray.remote decorator the function as an Actor
@ray.remote
def flip(numTimes):
    results = 0
    for i in range(0,numTimes):
        rdm = random.randint(0,1)
        if rdm == 1:
            results += 1
    return results/numTimes

if __name__ == "__main__":
    
    name = [10, 100, 1000,10000, 100000, 1000000]
    start_time = time.time()
    for x in name:
        print(flop(x))
    non_ray = time.time() - start_time
    print("Non-Ray took: {}".format(non_ray))


    ray.init()
    # ray.put() creates a Python obect
    n = ray.put(name)
    print(n)

    lst = []
    start_time = time.time()
    # For flip.remote()
    # ray.get() retreives the object id
    # 1. A node in the cluster is chosen and a worker process 
    # is created on that node (by the local scheduler on that node) 
    # for the purpose of running methods called on the actor.
    # 2. A Counter object is created on that worker and the Counter 
    # constructor is run.
    for x in name:
        lst.append(flip.remote(x))
    ray_time = time.time() - start_time
    print(ray.get(lst))
    print("Ray took: {}".format(ray_time))

    if non_ray == ray_time:
        print("Equal time")
    elif non_ray < ray_time:
        print("Non-ray was faster by {}".format(ray_time-non_ray))
    elif non_ray > ray_time:
        print("Ray was faster by {}".format(non_ray-ray_time))