import socket

SERVER_IP = "127.0.0.1"
SERVER_PORT = 12345
MSG_MAX_SIZE = 256 * 256


def send_to_server(message: str) -> None:
    """
    Function to communicate with a server. Send a message to it and print the
    response
    :param message: A string message to send
    :return: No return
    """
    # Set up a client socket. By using a 'with' statement we can leave out
    # explicitly closing the connection, as this will be done automatically
    # once the with statement completes
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        # Connect this socket to the given IP and PORT. Note these are the
        # same as where we have run the server, as that is what we wish to
        # connect to
        client_socket.connect((SERVER_IP, SERVER_PORT))

        # Assemble the request as a series of bytes and send along the
        # connection
        request = bytearray(message.encode())
        client_socket.sendall(request)

        # Receive a response and print it
        response = client_socket.recv(MSG_MAX_SIZE)

        # If utf-8 compatible response
        try:
            print(response.decode('utf-8'))
            client_socket.close()
            return
        except Exception as e:
            pass
        
        # If not utf-8 compatible
        response_split = response.split(b'\r\n\r\n')
        response_headers = response_split[0].decode('utf-8')
        payload = response_split[1]

        # Uncompress if compressed
        if "Content-Encoding: gzip" in response_headers:
            import gzip
            payload = gzip.decompress(payload)
    

        # Handle image responses

        # If jpg
        if "Content-Type: image/jpg" in response_headers:
            with open('downloaded_image.jpg', 'wb') as file:
                file.write(payload)
            payload = b"Image written to downloaded_image.jpg"

        # If png
        if "Content-Type: image/png" in response_headers:
            with open('downloaded_image.png', 'wb') as file:
                file.write(payload)
            payload = b"Image written to downloaded_image.png"
        payload = payload.decode('utf-8')

        print(response_headers + "\r\n\r\n" + payload)

        client_socket.close()

    

if __name__ == "__main__":
    # Send a message to a server

    requests = [
        # 0. Correct request: root folder, encoded
        # "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 1. Correct request, specific textfile
        "GET /textfile HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 2. Correct request, no encoding
        "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 3. Correct request, jpg requested
        "GET /Dog_meme HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */jpg\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Moilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 4. Correct request, png requested
        "GET /Dog_meme HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */png\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 5. Modified since, should not return the file
        "GET /Dog_meme HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */jpg\r\nAccept-Encoding: gzip,deflate\r\nIf-Modified-Since: Sat, 07 Jan 2023 14:21:00 GMT\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 6. Unmodified since, should not return the file
        "GET /Dog_meme HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */jpg\r\nAccept-Encoding: gzip,deflate\r\nIf-Unmodified-Since: Fri, 06 Jan 2023 14:21:00 GMT\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 7. Unmodified since, should not return the file
        "GET /Dog_meme HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */jpg\r\nAccept-Encoding: gzip,deflate\r\nIf-Unmodified-Since: Fri, 07 Jan 2023 14:21:00 GMT\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 8. Unsupported method
        "POST / HTTP/1.1\r\nHost: 127.0.0.1\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 9. Wrong host
        "GET / HTTP/1.1\r\nHost: 127.0.0.6\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n",

        # 10. Wrong HTTP version
        "GET / HTTP/1.0\r\nHost: 127.0.0.6\r\nAccept: */*\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13\r\nConnection: keep-alive\r\n\r\n"
    ]

    for i in range(len(requests)):
        print(f"------------------------------\n\n## Response for request no. {i}:\n")
        send_to_server(requests[i])