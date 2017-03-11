
# pumpkinbot

__Hack-o'-Lantern '13__

![](./pumpkinbot.gif)

## Introduction

This project was born of three requirements:

1. The [@blip](https://twitter.com/blip) office has an IRC server that people actually use
2. The [@blip](https://twitter.com/blip) IRC server has a bot - plusplusbot - that keeps score when people get ++'s and --'s
3. [@rickyrobinett](https://twitter.com/rickyrobinett) invited me to Hack-o-Latern '13 and i couldn't say no

so the goal of this project is simple: announce plusplusbot awards via an LED-illuminated hack-o-lantern

## Code

#### IRC gateway

Node.js app that monitors IRC for plusplusbot awards and relays them to an Arduino:

* `./pumpkinbot_js`


#### LED controller

Ethernet shield -> Arduino -> LED-laden pumpkin:

* `./pumpkinbot_cc`



