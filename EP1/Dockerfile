#FROM ubuntu
FROM gcc:4.9

ADD . /app

#RUN apt-get update && \
#    apt-get -y install gcc && \
#    rm -rf /var/lib/apt/lists/*
#RUN ls -la
RUN gcc -o ./app/app ./app/script1.c

# Run app.py when the container launches
CMD ["./app/app"]