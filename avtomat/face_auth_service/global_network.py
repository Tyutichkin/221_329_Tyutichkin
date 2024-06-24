from pyngrok import ngrok

# Set Ngrok authtoken
ngrok.set_auth_token("Ключ, полученный на сайте")

# Start ngrok tunnel to expose local server
public_url = ngrok.connect(8765)

print("Public URL:", public_url)

input("Press Enter to stop the server and close the tunnel...")
