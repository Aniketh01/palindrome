# -*- coding: utf-8 -*-
from __future__ import unicode_literals

import os

from django.contrib.auth import authenticate, login
from django.urls import reverse_lazy
from django.views import generic
from django.shortcuts import render
from django.core.management import call_command
from .forms import loggerForm
from .models import loggerModel


class add_logger(generic.CreateView):
    template_name = 'logger.html'

    def get(self, request):
        form = loggerForm()
        return render(request, self.template_name, {'form': form})
    
    def post(self, request):
        form= logger(request.POST)
        args = {'form': form, 'text': text}
        return render(request, self.template_name, args)
    
    def search(request):
        if 'inp' in request.GET:
            print(request.GET)
            message = 'You submitteds: %r' % request.GET['inp']
        else:
            message = 'You submitted nothing!'    
        return render(request , 'test.html')

    def profile_page(request, username=None):
        user = User.objects.get(username=username)
        message = request.GET.get('message')