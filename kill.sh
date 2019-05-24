#!/bin/bash
docker kill `docker ps --format '{{.Names}}'`