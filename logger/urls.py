# accounts/urls.py
from django.urls import path
from . import views
from django.conf.urls import url

urlpatterns = [
    path('', views.add_logger.as_view()),
    url(r'^search-form/$', views.add_logger.search_form),
    url(r'^search/$', views.add_logger.search),
   # path(r'^/(?P<inp>\w{0,50})/$', views.add_logger.profile_page,),
]
