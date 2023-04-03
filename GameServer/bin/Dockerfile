FROM ubuntu
WORKDIR /game/
COPY ./game.out /game
COPY ./docker_game_start.sh /game
EXPOSE 8888
ENTRYPOINT ["./docker_game_start.sh"]