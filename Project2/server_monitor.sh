#!/bin/bash

# Default thresholds
CPU_THRESHOLD=80
MEM_THRESHOLD=80
DISK_THRESHOLD=80

LOG_FILE="server_health.log"
MONITORING=false

# FUNCTION: Get CPU Usage

get_cpu_usage() {
    cpu=$(top -bn1 | grep "Cpu(s)" | awk '{print 100 - $8}')
    echo ${cpu%.*}
}

# FUNCTION: Get Memory Usage

get_memory_usage() {
    mem=$(free | awk '/Mem:/ {printf("%.0f"), $3/$2 * 100}')
    echo $mem
}

# FUNCTION: Get Disk Usage

get_disk_usage() {
    disk=$(df / | awk 'NR==2 {print $5}' | sed 's/%//')
    echo $disk
}

# FUNCTION: Show Processes

show_processes() {
    echo "Top 5 Processes:"
    ps -eo pid,ppid,cmd,%mem,%cpu --sort=-%cpu | head -n 6
}

# FUNCTION: Log Message

log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> $LOG_FILE
}

# FUNCTION: Check Alerts

check_alerts() {
    cpu=$(get_cpu_usage)
    mem=$(get_memory_usage)
    disk=$(get_disk_usage)

    if [ "$cpu" -gt "$CPU_THRESHOLD" ]; then
        echo " High CPU Usage: $cpu%"
        log_message "ALERT: High CPU Usage: $cpu%"
    fi

    if [ "$mem" -gt "$MEM_THRESHOLD" ]; then
        echo " High Memory Usage: $mem%"
        log_message "ALERT: High Memory Usage: $mem%"
    fi

    if [ "$disk" -gt "$DISK_THRESHOLD" ]; then
        echo " High Disk Usage: $disk%"
        log_message "ALERT: High Disk Usage: $disk%"
    fi
}

# FUNCTION: Display System Health

display_status() {
    cpu=$(get_cpu_usage)
    mem=$(get_memory_usage)
    disk=$(get_disk_usage)

    echo "=============================="
    echo "   SYSTEM HEALTH STATUS"
    echo "=============================="
    echo "CPU Usage: $cpu%"
    echo "Memory Usage: $mem%"
    echo "Disk Usage: $disk%"
    echo "------------------------------"

    show_processes

    log_message "Status Checked: CPU=$cpu% MEM=$mem% DISK=$disk%"
    check_alerts
}

# FUNCTION: Configure Thresholds

configure_thresholds() {
    echo "Enter CPU Threshold:"
    read cpu
    echo "Enter Memory Threshold:"
    read mem
    echo "Enter Disk Threshold:"
    read disk

    # Validate input
    if [[ "$cpu" =~ ^[0-9]+$ && "$mem" =~ ^[0-9]+$ && "$disk" =~ ^[0-9]+$ ]]; then
        CPU_THRESHOLD=$cpu
        MEM_THRESHOLD=$mem
        DISK_THRESHOLD=$disk
        echo "Thresholds Updated!"
    else
        echo " Invalid input. Please enter numbers only."
    fi
}

# FUNCTION: View Logs

view_logs() {
    if [ -f "$LOG_FILE" ]; then
        cat $LOG_FILE
    else
        echo "No logs found."
    fi
}

# FUNCTION: Clear Logs

clear_logs() {
    > $LOG_FILE
    echo "Logs cleared!"
}

# FUNCTION: Start Monitoring

start_monitoring() {
    MONITORING=true
    echo "Monitoring started... Press Ctrl+C to stop."

    while $MONITORING
    do
        display_status
        sleep 5
    done
}

# MAIN MENU

while true
do
    echo ""
    echo "====== SERVER MONITOR MENU ======"
    echo "1. Show System Health"
    echo "2. Configure Thresholds"
    echo "3. View Logs"
    echo "4. Clear Logs"
    echo "5. Start Monitoring"
    echo "6. Exit"
    echo "================================"
    echo "Choose an option:"

    read choice

    case $choice in
        1) display_status ;;
        2) configure_thresholds ;;
        3) view_logs ;;
        4) clear_logs ;;
        5) start_monitoring ;;
        6) echo "Exiting..."; exit 0 ;;
        *) echo " Invalid option. Try again." ;;
    esac
done