def query_price(stocks):
    try:
        import sys
        sys.path.append('C:\\Users\\TalG\\Miniconda3\\Lib\\site-packages')
        sys.path.append('C:\\Users\\TalG\\Miniconda3\\DLLs')

        import requests
        access_key = get_access_key()
        base_url = 'https://api.unibit.ai/api/realtimestock/'
        output = []

        for stock in stocks:
            url = base_url + stock + '?size=1&AccessKey=' + access_key
            resp = requests.get(url).json()
            price = resp['Realtime Stock price'][0]['price']
            output.append(float(price))

        return output

    except Exception as e:
        with open('./utilfiles/stderr.txt', 'w+') as f:
            f.write(str(e))
            f.close()


def get_access_key():
    with open('./utilfiles/key.txt') as f:
        key = f.read()
        f.close()

    return key