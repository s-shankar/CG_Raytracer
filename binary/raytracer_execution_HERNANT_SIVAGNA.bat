@echo off
echo Assignment 02 by Tristan Hernant and Shankar SIVAGNA
start /d %~dp0 RayTracer.exe scene01-zbuffer.yaml
start /d %~dp0 RayTracer.exe scene01-normal.yaml
start /d %~dp0 RayTracer.exe scene01-normal-2nd_angle.yaml
start /d %~dp0 RayTracer.exe customscene-zbuffer.yaml
echo Done
pause