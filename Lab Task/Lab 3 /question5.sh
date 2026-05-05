#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory_to_backup>"
    exit 1
fi

source_dir=$1

if [ ! -d "$source_dir" ]; then
    echo "Error: Directory does not exist"
    exit 1
fi

backup_dir="/tmp/backups"
mkdir -p $backup_dir

current_date=$(date '+%Y-%m-%d')

backup_folder="$backup_dir/backup_$current_date"
mkdir -p $backup_folder

echo "Starting backup..."
echo "Source: $source_dir"
echo "Destination: $backup_folder"
echo ""

cp -r $source_dir/* $backup_folder/

files=$(find $backup_folder -type f | wc -l)
dirs=$(find $backup_folder -type d | wc -l)

echo "Backup completed!"
echo "Files backed up: $files"
echo "Directories backed up: $dirs"
echo "Backup saved to: $backup_folder"
