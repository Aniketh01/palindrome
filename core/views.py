# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.contrib.auth import authenticate, login
from django.urls import reverse_lazy
from django.views import generic
from django.shortcuts import render


class add_IP(generic.CreateView):
    success_url = reverse_lazy('login')
    template_name = 'add_ip.html'
