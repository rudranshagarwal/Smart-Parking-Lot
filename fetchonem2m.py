
import matplotlib.pyplot as plt
import requests
uri_cnt = []
c = []
ans = []
uri_cnt.append("http://~/in-cse/in-name/ultrasonic/parking1?rcn=4")
uri_cnt.append("http://127.0.0.1:8080/~/in-cse/in-name/ultrasonic/parking2?rcn=4")
uri_cnt.append("http://127.0.0.1:8080/~/in-cse/in-name/ultrasonic/parking3?rcn=4")
uri_cnt.append("http://127.0.0.1:8080/~/in-cse/in-name/ultrasonic/parking4?rcn=4")

for j in range(4):
    print(f"j={j+1}")
    ans.append(0)
    header = {
    'X-M2M-Origin': 'admin:admin',
    'Content-type': 'application/json'
    }
    response = requests.get(uri_cnt[j], headers=header)
    data = response.json()
    data = data["m2m:cnt"]["m2m:cin"]

    x = []
    y = []

    c.append(0)
    for i in data:
        if c[j] >= 1:
            ans[j] = float(ans[j])+  float(i["con"])
            x.append(int(c[j]))
            y.append(float(i["con"]))
            pass
        c[j] = c[j] + 1
    
    print(ans[j])
    
    

    plt.plot(x, y)
    plt.xlabel('car number')
    plt.ylabel('Time')
    plt.title(j)
    plt.show()
    # print(response.text)
    # print(type(response))

print(max(ans))
print(ans.index(max(ans)))
print(max(c))
print(c.index(max(c)))

