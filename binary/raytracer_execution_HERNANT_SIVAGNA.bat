@echo off
echo Assignment 05 by Tristan Hernant and Shankar SIVAGNA
start /d %~dp0 RayTracer.exe scene01-texture-ss-reflect-lights-shadows.yaml
start /d %~dp0 RayTracer.exe scene01-texture-ss-reflect-lights-shadows_2.yaml
start /d %~dp0 RayTracer.exe scene01-gooch.yaml
start /d %~dp0 RayTracer.exe scene01-gooch-reflect-lights-shadows.yaml
echo Done
pause