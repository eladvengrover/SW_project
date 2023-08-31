import math
import sys


def check_validation(K, N, iter):
    is_valid = True
    if not K.isdigit() or int(K) < 2 or int(K) > N - 1:
        print("Invalid number of clusters!")
        is_valid = False
    if not iter.isdigit() or int(iter) < 2 or int(iter) > 999:
        print("Invalid maximum iteration!")
        is_valid = False
    if not is_valid:
        exit()


def read_data_points(file_path):
    points = []
    with open(file_path, 'r') as f:
        for line in f:
            data_point = tuple(map(float, line.strip().split(',')))
            points.append(data_point)
    return points


def init_clusters(centroids, data_points, K):
    for point in data_points:
        if len(centroids) == K:
            break
        centroids.append(point)


def euclidean_dis(dp1, dp2):
    dis = 0
    for x, y in zip(dp1, dp2):
        dis += (x-y) ** 2

    return math.sqrt(dis)


def find_cluster(centroids, data_point):
    smallest_dis = float('inf')
    cur_centroid_index = -1
    for centroid in centroids:
        cur_dis = euclidean_dis(data_point, centroid)
        if cur_dis < smallest_dis:
            smallest_dis = cur_dis
            cur_centroid_index = centroids.index(centroid)

    return cur_centroid_index


def calculate_clusters_centroid(data_points, cluster):
    curr_sum = [0 for i in range(len(data_points[0]))]
    for point in cluster:
        for i in range(len(point)):
            curr_sum[i] += point[i]
    return tuple(x / len(cluster) for x in curr_sum)


def k_means_alg(centroids, data_points, K, epsilon, iter):
    convergence = True
    output = [[] for i in range(K)]
    for i in range(iter):
        # Assign each point to the closest centroid
        clusters = [[] for i in range(K)]
        for data_point in data_points:
            closest_centroid = find_cluster(centroids, data_point)
            clusters[closest_centroid].append(data_point)

        # Calculate the new centroids as the mean of the points in each cluster
        new_centroids = []
        output = clusters
        for cluster in clusters:
            prev_centroid = centroids[clusters.index(cluster)]
            if cluster:
                new_centroid = calculate_clusters_centroid(data_points, cluster)
                new_centroids.append(new_centroid)
                if euclidean_dis(new_centroid, prev_centroid) >= epsilon:
                    convergence = False
            else:
                # If a cluster is empty, keep its previous centroid
                new_centroids.append(prev_centroid)

        # Check if the centroids have moved
        if convergence:
            break

        centroids = new_centroids
    return output


def get_clusters(k, file_name):
    iter = 200
    epsilon = 0.001
    data_points = read_data_points(file_name)
    N = len(data_points)


    centroids = []
    init_clusters(centroids, data_points, k)
    return k_means_alg(centroids, data_points, k, epsilon, iter)