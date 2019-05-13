# -*- coding: utf-8 -*-
from __future__ import unicode_literals

import os

from django.contrib.auth import authenticate, login
from django.urls import reverse_lazy
from django.views import generic
from django.shortcuts import render
from .forms import addIPForm
from .models import IP
from django.core.management import call_command


class add_IP(generic.CreateView):
    template_name = 'add_ip.html'

    def get(self, request):
            form = addIPForm()
            return render(request, self.template_name, {'form': form})

    def post(self, request):
        form = addIPForm(request.POST)
        if form.is_valid():
            ipaddr = form.save(commit=False)
            ipaddr.user = request.user
            ipaddr.save()

            text = form.cleaned_data['ipaddr']
            form = addIPForm()

            value = str(IP.objects.latest('id')).split(':')
            addr = value[0]
            mode = value[1]

            cmd = 'python3 udp.py' + " " + addr + " " + mode

            os.system(cmd)

        args = {'form': form, 'text': text}
        return render(request, self.template_name, args)

