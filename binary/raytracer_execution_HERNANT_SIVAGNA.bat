@echo off
echo Assignment 03 by Tristan Hernant and Shankar SIVAGNA
start /d %~dp0 RayTracer.exe scene01-shadows.yaml
start /d %~dp0 RayTracer.exe scene01-reflect-shadows.yaml
start /d %~dp0 RayTracer.exe scene01-lights-shadows.yaml
start /d %~dp0 RayTracer.exe scene01-reflect-lights-shadows.yaml
echo Done
pause