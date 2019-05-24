#!/bin/bash
docker exec -ti `docker ps --format '{{.Names}}'` bash