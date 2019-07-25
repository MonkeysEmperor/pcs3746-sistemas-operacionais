#!/bin/bash
sudo docker kill `sudo docker ps --format '{{.Names}}'`
