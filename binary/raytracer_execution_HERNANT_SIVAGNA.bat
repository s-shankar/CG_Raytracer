@echo off
echo Assignment 04 by Tristan Hernant and Shankar SIVAGNA
start /d %~dp0 RayTracer.exe scene01-normal_2nd-angle.yaml
start /d %~dp0 RayTracer.exe scene01-normal_3rd-angle.yaml
start /d %~dp0 RayTracer.exe scene01-reflect-lights-shadows_2nd-angle.yaml
start /d %~dp0 RayTracer.exe scene01-reflect-lights-shadows_ss4.yaml
echo Done
pause