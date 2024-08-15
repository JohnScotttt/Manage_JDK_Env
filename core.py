#
# Created by JohnScotttt on 2024/8/15.
# Version 1.0
#

import yaml
import os
import sys
import argparse
import winreg
import subprocess

version = '1.0'
build_t = '2024-8-15'

if not os.path.exists('env.yml'):
    with open('env.yml', 'w') as f:
        yaml.dump({}, f)
try:
    java_env_dict = yaml.load(open('env.yml'), Loader=yaml.FullLoader)
except Exception:
    print('Failed to load "env.yml". Try to delete it and run the program again.')
    sys.exit(0)
Env = winreg.CreateKey(winreg.HKEY_CURRENT_USER, r'Environment')
Path = winreg.QueryValueEx(Env, 'Path')[0]
if "%JAVA_HOME%" not in Path:
    Path += ';%JAVA_HOME%'
    winreg.SetValueEx(Env, 'Path', 0, winreg.REG_EXPAND_SZ, Path)
winreg.CloseKey(Env)


def list_env():
    if not java_env_dict:
        print('No Java environment found.')
        sys.exit(0)
    max_key_len = max(8, max(len(key) for key in java_env_dict.keys()))
    max_value_len = max(8, max(len(value) for value in java_env_dict.values()))
    print(f'{"Env_Name":<{max_key_len}} | {"Env_Path":<{max_value_len}}')
    print('-' * (max_key_len) + "-|-" + "-" * (max_value_len))
    for key, value in java_env_dict.items():
        print(f'{key:<{max_key_len}} | {value:<{max_value_len}}')


def add_env(env_name, env_path):
    if env_name in java_env_dict.keys():
        print(f'Name {env_name} already exists.')
        sys.exit(0)
    if os.path.exists(env_path):
        java_env_dict[env_name] = env_path
        with open('env.yml', 'w') as f:
            yaml.dump(java_env_dict, f)
        print(f'Java environment {env_name} added successfully.')
    else:
        print(f'Path {env_path} does not exist.')


def remove_env(env_name):
    if env_name in java_env_dict.keys():
        java_env_dict.pop(env_name)
        with open('env.yml', 'w') as f:
            yaml.dump(java_env_dict, f)
        print(f'Java environment {env_name} removed successfully.')
    else:
        print(f'Name {env_name} does not exist.')


def enable_env(env_name):
    if env_name in java_env_dict.keys():
        prosess = subprocess.Popen(f'setx JAVA_HOME "{java_env_dict[env_name]}\\bin"',
                                   shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        prosess
        print(f'Java environment {env_name} enabled successfully.')
    else:
        print(f'Name {env_name} does not exist.')


def disable_env():
    prosess = subprocess.Popen('setx JAVA_HOME ""',
                               shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    prosess
    print('Java environment is now disabled.')


if __name__ == '__main__':
    try:
        args = argparse.ArgumentParser(usage=argparse.SUPPRESS,
                                       add_help=False)
        args.add_argument('-h', '--help',
                          action='help',
                          default=argparse.SUPPRESS,
                          help='Show this help message and exit')
        args.add_argument('-l', '--list',
                          action='store_true',
                          help='List all Java environment')
        args.add_argument('-a', '--add',
                          type=str,
                          nargs=2,
                          metavar=('Env_Name', 'Env_Path'),
                          help='Add a new Java environment')
        args.add_argument('-r', '--remove',
                          type=str,
                          metavar='Env_Name',
                          help='Remove a Java environment')
        args.add_argument('-e', '--enable',
                          type=str,
                          metavar='Env_Name',
                          help='Enable a Java environment')
        args.add_argument('-d', '--disable',
                          action='store_true',
                          help='Disable Java environment')
        args.add_argument('-v', '--version',
                          action='store_true',
                          help='Show version')
        args = args.parse_args()
    except SystemExit:
        print('Use -h or --help for help.')
        sys.exit(0)

    if len(sys.argv) == 1:
        print('No arguments provided. Use -h or --help for help.')
        sys.exit(0)

    if args.list:
        list_env()

    if args.add:
        env_name, env_path = args.add
        add_env(env_name, env_path)

    if args.remove:
        env_name = args.remove
        remove_env(env_name)

    if args.enable:
        env_name = args.enable
        enable_env(env_name)

    if args.disable:
        disable_env()

    if args.version:
        print(f'MJE core version {version} {build_t}')
