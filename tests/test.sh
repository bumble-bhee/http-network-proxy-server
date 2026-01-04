#!/bin/bash

echo "Starting proxy automated tests..."

echo ""
echo "1) Testing forwarding (example.com)"
curl -s -o /dev/null -w "%{http_code}\n" -x localhost:8888 http://example.com

echo ""
echo "2) Testing blocking (httpbin.org)"
curl -s -o /dev/null -w "%{http_code}\n" -x localhost:8888 http://httpbin.org/get

echo ""
echo "If you saw 200 for example.com and 403 for httpbin.org:"
echo "✓ Automated test script executed successfully"
